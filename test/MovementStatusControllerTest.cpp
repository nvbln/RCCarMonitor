#include "controllers/MovementStatusController.h"
#include "views/IStatusView.h"

#include "mock/MockBluetoothDevice.h"
#include "mock/MockGattCharacteristic.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::NiceMock;
using ::testing::Return;

class MockStatusView : public IStatusView {
public:
  MOCK_METHOD(void, update, (const std::string status), (override));
  MOCK_METHOD(void, draw, (), (override));
};

TEST(MovementStatusControllerTests, whenStatusIsUpdatedUpdateView) {
  auto mockGattChar = std::make_shared<NiceMock<MockGattCharacteristic>>();

  // Check if it can display different statuses (0 and 1) and an incorrect one (-1).
  EXPECT_CALL(*mockGattChar, read)
      .WillOnce(Return(std::vector<uint8_t>{0}))
      .WillOnce(Return(std::vector<uint8_t>{1}))
      .WillOnce(Return(std::vector<uint8_t>{250}));

  auto mockDevice = std::make_shared<NiceMock<MockBluetoothDevice>>();
  ON_CALL(*mockDevice, findCharacteristic).WillByDefault(Return(std::make_optional(mockGattChar)));

  auto mockStatusView = std::make_shared<NiceMock<MockStatusView>>();
  EXPECT_CALL(*mockStatusView, update("Forwards"));
  EXPECT_CALL(*mockStatusView, update("Backwards"));
  EXPECT_CALL(*mockStatusView, update("Unknown"));

  std::string mockId = "MOCK_ID";
  MovementStatusController controller =
      MovementStatusController(mockDevice, mockStatusView, mockId);

  controller.update();
  controller.update();
}
