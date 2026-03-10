#include "controllers/DriveLockController.h"
#include "views/IToggleableView.h"

#include "mock/MockBluetoothDevice.h"
#include "mock/MockGattCharacteristic.h"

#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::NiceMock;
using ::testing::Return;

static const std::vector<uint8_t> BytesTrue = {0x01};
static const std::vector<uint8_t> BytesFalse = {0x00};

class MockToggleableView : public IToggleableView {
public:
  MOCK_METHOD(void, update, (const bool on), (override));
  MOCK_METHOD(void, subscribe, (Callback callback), (override));
  MOCK_METHOD(void, draw, (), (override));
  MOCK_METHOD(void, enable, (), (override));
  MOCK_METHOD(void, disable, (), (override));
};

TEST(DriveLockControllerTests, whenButtonClickedInViewWriteToChar) {
  auto mockGattChar = std::make_shared<NiceMock<MockGattCharacteristic>>();
  EXPECT_CALL(*mockGattChar, write(BytesTrue));
  EXPECT_CALL(*mockGattChar, write(BytesFalse));

  auto mockDevice = std::make_shared<NiceMock<MockBluetoothDevice>>();
  ON_CALL(*mockDevice, findCharacteristic).WillByDefault(Return(std::make_optional(mockGattChar)));

  auto mockToggleable = std::make_shared<NiceMock<MockToggleableView>>();
  IToggleableView::Callback callback;
  ON_CALL(*mockToggleable, subscribe).WillByDefault([&](IToggleableView::Callback cb) {
    callback = cb;
  });

  std::string mockId = "MOCK_ID";
  DriveLockController controller = DriveLockController(mockDevice, mockToggleable, mockId);

  // Trigger callback
  callback(true);
  callback(false);
}

TEST(DriveLockControllerTests, whenPropertyChangedInCharUpdateView) {
  auto mockGattChar = std::make_shared<NiceMock<MockGattCharacteristic>>();
  EXPECT_CALL(*mockGattChar, read).WillOnce(Return(BytesTrue)).WillOnce(Return(BytesFalse));

  auto mockDevice = std::make_shared<NiceMock<MockBluetoothDevice>>();
  ON_CALL(*mockDevice, findCharacteristic).WillByDefault(Return(std::make_optional(mockGattChar)));

  auto mockToggleable = std::make_shared<NiceMock<MockToggleableView>>();
  EXPECT_CALL(*mockToggleable, update(true));
  EXPECT_CALL(*mockToggleable, update(false));

  std::string mockId = "MOCK_ID";
  DriveLockController controller = DriveLockController(mockDevice, mockToggleable, mockId);

  // Only need one update() call to verify true and false
  // because the constructor also calls update().
  controller.update();
}
