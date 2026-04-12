#include "controllers/MovementStatusController.h"
#include "views/IStatusView.h"

#include "mock/MockGattCharacteristicHandler.h"

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
  auto mockHandler = NiceMock<MockGattCharacteristicHandler>();

  // Check if it can display different statuses (0 and 1) and an incorrect one (-1).
  EXPECT_CALL(mockHandler, read)
      .WillOnce(Return(std::vector<uint8_t>{0}))
      .WillOnce(Return(std::vector<uint8_t>{1}))
      .WillOnce(Return(std::vector<uint8_t>{250}));

  auto mockStatusView = NiceMock<MockStatusView>();
  EXPECT_CALL(mockStatusView, update("Forwards"));
  EXPECT_CALL(mockStatusView, update("Backwards"));
  EXPECT_CALL(mockStatusView, update("Unknown"));

  MovementStatusController controller = MovementStatusController(&mockStatusView, &mockHandler);

  controller.update();
  controller.update();
}
