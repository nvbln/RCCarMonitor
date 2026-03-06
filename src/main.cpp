#include <iostream>
#include <sdbus-c++/sdbus-c++.h>

#include "DBusBluetoothManager.h"
#include "proxies/DBusObjectManagerProxy.h"

#include "controllers/BluetoothDeviceController.h"
#include "views/ItemPickerView.h"

#include <chrono>
#include <thread>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

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

  std::shared_ptr<IGattCharacteristic> characteristic = nullptr;
  std::shared_ptr<IBluetoothDevice> device = nullptr;
  std::shared_ptr<ItemPickerView> itemPicker =
      std::make_shared<ItemPickerView>(std::vector<std::string>());
  BluetoothDeviceController deviceController =
      BluetoothDeviceController(bluetoothManager, itemPicker);
  deviceController.subscribe([&](std::shared_ptr<IBluetoothDevice> pickedDevice) {
    device = pickedDevice;
    device->connect();

    std::cout << "Connected to device? " << device->isConnected() << std::endl;
    auto characteristicPtr = device->findCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214");
    if (characteristicPtr) {
      characteristic = *characteristicPtr;
    }
  });

  std::vector<uint8_t> bytesTrue = {0x01};
  std::vector<uint8_t> bytesFalse = {0x00};
  bool readCurrentValue = false;
  bool turnedLedOn = false;
  bool turnedLedOff = false;
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
    } else if (characteristic == nullptr) {
      ImGui::Begin("Status");
      ImGui::TextWrapped("Characteristic could not be found");
      ImGui::End();
    } else if (characteristic && !readCurrentValue) {
      std::vector<uint8_t> currentValue = characteristic->read();

      readCurrentValue = true;

      ImGui::Begin("Status");
      if (currentValue != bytesTrue) {
        ImGui::TextWrapped("LED is off");
      } else {
        ImGui::TextWrapped("Led is on");
      }
      ImGui::End();
    } else if (characteristic && readCurrentValue && !turnedLedOn) {
      characteristic->write(bytesTrue);
      turnedLedOn = true;

      ImGui::Begin("Status");
      ImGui::TextWrapped("Turned LED on");
      ImGui::End();

    } else if (characteristic && readCurrentValue && turnedLedOn && !turnedLedOff) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));
      characteristic->write(bytesFalse);
      turnedLedOff = true;

      ImGui::Begin("Status");
      ImGui::TextWrapped("Turned LED off");
      ImGui::End();
    } else if (characteristic && readCurrentValue && turnedLedOn && turnedLedOff &&
               device->isConnected()) {
      ImGui::Begin("Status");
      ImGui::TextWrapped("Disconnecting...");
      ImGui::End();
      device->disconnect();
    } else if (characteristic && readCurrentValue && turnedLedOn && turnedLedOff &&
               !device->isConnected()) {
      ImGui::Begin("Status");
      ImGui::TextWrapped("Device disconnected");
      ImGui::End();
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
