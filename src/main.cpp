#include <iostream>
#include <sdbus-c++/sdbus-c++.h>

#include "DBusBluetoothManager.h"
#include "proxies/DBusObjectManagerProxy.h"

#include "controllers/BluetoothDeviceController.h"
#include "controllers/DriveLockController.h"
#include "controllers/MovementStatusController.h"
#include "views/ItemPickerView.h"
#include "views/StatusView.h"
#include "views/ToggleableView.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

static constexpr const char *DRIVE_LOCK_CHAR_ID = "19B10001-E8F2-537E-4F6C-D104768A1214";
static constexpr const char *MOVEMENT_STATUS_ID = "A92E318E-9EC4-4DB5-A861-7D0B6B77A2A1";

int main() {
  // Create a proxy for the BlueZ ObjectManager.
  auto connection = sdbus::createSystemBusConnection();
  sdbus::ServiceName destination{"org.bluez"};
  sdbus::ObjectPath objectPath{"/"};

  connection->enterEventLoopAsync();

  // TODO: Error handling
  std::shared_ptr<IBluetoothManager> bluetoothManager;
  try {
    auto proxy =
        std::make_shared<DBusObjectManagerProxy>(*connection, destination, std::move(objectPath));
    bluetoothManager = std::make_shared<DBusBluetoothManager>(proxy);
  } catch (const sdbus::Error &e) {
    std::cerr << "Call failed: " << e.getName() << " - " << e.getMessage() << std::endl;
    return 1;
  }

  // For all adapters, scan for new Bluetooth devices.
  auto adapters = bluetoothManager->getAdapters();
  for (auto adapter : adapters) {
    adapter->startDiscovery();
  }

  // Set up the GUI
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(500, 400, "MyApp", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Disable the imgui.ini
  ImGui::GetIO().IniFilename = nullptr;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  std::unique_ptr<DriveLockController> driveLockController = nullptr;
  std::unique_ptr<MovementStatusController> movementStatusController = nullptr;
  std::shared_ptr<IToggleableView> toggleableView = std::make_shared<ToggleableView>(false);
  std::shared_ptr<IStatusView> statusView = std::make_shared<StatusView>();

  std::shared_ptr<IGattCharacteristic> characteristic = nullptr;
  std::shared_ptr<IBluetoothDevice> device = nullptr;
  std::shared_ptr<ItemPickerView> itemPicker =
      std::make_shared<ItemPickerView>(std::vector<std::string>());
  BluetoothDeviceController deviceController =
      BluetoothDeviceController(bluetoothManager, itemPicker);
  deviceController.subscribe([&](std::shared_ptr<IBluetoothDevice> pickedDevice) {
    device = pickedDevice;
    device->connect();
  });

  std::vector<uint8_t> bytesTrue = {0x01};
  std::vector<uint8_t> bytesFalse = {0x00};
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    // ImGui::ShowDemoWindow();
    if (device == nullptr) { // Pick a bluetooth device.
      deviceController.update();
      itemPicker->draw();
    } else {
      if (driveLockController == nullptr) {
        driveLockController =
            std::make_unique<DriveLockController>(device, toggleableView, DRIVE_LOCK_CHAR_ID);
      } else {
        driveLockController->update();
      }

      if (movementStatusController == nullptr) {
        movementStatusController =
            std::make_unique<MovementStatusController>(device, statusView, MOVEMENT_STATUS_ID);
      } else {
        movementStatusController->update();
      }
      toggleableView->draw();
      statusView->draw();
    }

    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
