#include "controllers/DriveLockController.h"
#include "views/IToggleableView.h"

#include "mock/MockGattCharacteristicHandler.h"

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
  auto mockHandler = NiceMock<MockGattCharacteristicHandler>();
  EXPECT_CALL(mockHandler, write(BytesTrue));
  EXPECT_CALL(mockHandler, write(BytesFalse));

  auto mockToggleable = NiceMock<MockToggleableView>();
  IToggleableView::Callback callback;
  ON_CALL(mockToggleable, subscribe).WillByDefault([&](IToggleableView::Callback cb) {
    callback = cb;
  });

  DriveLockController controller = DriveLockController(&mockToggleable, &mockHandler);

  // Trigger callback
  callback(true);
  callback(false);
}

TEST(DriveLockControllerTests, whenPropertyChangedInCharUpdateView) {
  auto mockHandler = NiceMock<MockGattCharacteristicHandler>();
  EXPECT_CALL(mockHandler, read).WillOnce(Return(BytesTrue)).WillOnce(Return(BytesFalse));

  auto mockToggleable = NiceMock<MockToggleableView>();
  EXPECT_CALL(mockToggleable, update(true));
  EXPECT_CALL(mockToggleable, update(false));

  DriveLockController controller = DriveLockController(&mockToggleable, &mockHandler);

  // Only need one update() call to verify true and false
  // because the constructor also calls update().
  controller.update();
}
