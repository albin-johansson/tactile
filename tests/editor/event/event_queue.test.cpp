// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/event/event_queue.hpp"

#include <gtest/gtest.h>

#include "tactile/core/container/string.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/editor/event/event_id.hpp"

namespace tactile::editor {
namespace {

struct E1 final
{
  constexpr static EventID kID = 0;
};

struct E2 final
{
  constexpr static EventID kID = 1;

  int x {};
  int y {};
};

struct E3 final
{
  constexpr static EventID kID = 2;

  String str {};
};

class EventQueueTest : public testing::Test
{
 public:
  void SetUp() override
  {
    m_queue.connect<E1>([this](const E1& event) {
      m_e1_history.push_back(event);
      m_id_history.push_back(E1::kID);
    });

    m_queue.connect<E2>([this](const E2& event) {
      m_e2_history.push_back(event);
      m_id_history.push_back(E2::kID);
    });

    m_queue.connect<E3>([this](const E3& event) {
      m_e3_history.push_back(event);
      m_id_history.push_back(E3::kID);
    });
  }

 protected:
  EventQueue m_queue {};
  Vector<E1> m_e1_history {};
  Vector<E2> m_e2_history {};
  Vector<E3> m_e3_history {};
  Vector<EventID> m_id_history {};
};

TEST_F(EventQueueTest, Clear)
{
  m_queue.enqueue<E1>();
  m_queue.enqueue<E2>(1, 2);
  m_queue.enqueue<E3>("foo");

  EXPECT_EQ(m_queue.size(), 3);

  m_queue.clear();

  EXPECT_EQ(m_queue.size(), 0);
}

TEST_F(EventQueueTest, Disconnect)
{
  m_queue.enqueue<E1>();
  m_queue.enqueue<E2>(1, 2);
  m_queue.enqueue<E3>("foo");

  m_queue.disconnect<E1>();

  EXPECT_EQ(m_queue.size(), 3);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 0);
  EXPECT_EQ(m_e3_history.size(), 0);
  EXPECT_EQ(m_id_history.size(), 0);

  m_queue.publish();

  EXPECT_EQ(m_queue.size(), 0);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 1);
  EXPECT_EQ(m_e3_history.size(), 1);

  ASSERT_EQ(m_id_history.size(), 2);
  EXPECT_EQ(m_id_history[0], E2::kID);
  EXPECT_EQ(m_id_history[1], E3::kID);
}

TEST_F(EventQueueTest, EnqueueSingleEvent)
{
  EXPECT_EQ(m_queue.size(), 0);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 0);
  EXPECT_EQ(m_e3_history.size(), 0);
  EXPECT_EQ(m_id_history.size(), 0);

  m_queue.enqueue<E1>();

  EXPECT_EQ(m_queue.size(), 1);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 0);
  EXPECT_EQ(m_e3_history.size(), 0);
  EXPECT_EQ(m_id_history.size(), 0);

  m_queue.publish();

  EXPECT_EQ(m_queue.size(), 0);
  EXPECT_EQ(m_e1_history.size(), 1);
  EXPECT_EQ(m_e2_history.size(), 0);
  EXPECT_EQ(m_e3_history.size(), 0);

  ASSERT_EQ(m_id_history.size(), 1);
  EXPECT_EQ(m_id_history[0], E1::kID);
}

TEST_F(EventQueueTest, EnqueueMultipleEvents)
{
  EXPECT_EQ(m_queue.size(), 0);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 0);
  EXPECT_EQ(m_e3_history.size(), 0);
  EXPECT_EQ(m_id_history.size(), 0);

  m_queue.enqueue<E1>();
  m_queue.enqueue<E2>(1, 2);
  m_queue.enqueue<E3>("foo");
  m_queue.enqueue<E2>(3, 4);
  m_queue.enqueue<E2>(5, 6);
  m_queue.enqueue<E1>();

  EXPECT_EQ(m_queue.size(), 6);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 0);
  EXPECT_EQ(m_e3_history.size(), 0);
  EXPECT_EQ(m_id_history.size(), 0);

  m_queue.publish();

  EXPECT_EQ(m_queue.size(), 0);
  EXPECT_EQ(m_e1_history.size(), 2);
  EXPECT_EQ(m_e2_history.size(), 3);
  EXPECT_EQ(m_e3_history.size(), 1);

  ASSERT_EQ(m_id_history.size(), 6);
  EXPECT_EQ(m_id_history[0], E1::kID);
  EXPECT_EQ(m_id_history[1], E2::kID);
  EXPECT_EQ(m_id_history[2], E3::kID);
  EXPECT_EQ(m_id_history[3], E2::kID);
  EXPECT_EQ(m_id_history[4], E2::kID);
  EXPECT_EQ(m_id_history[5], E1::kID);
}

TEST_F(EventQueueTest, Trigger)
{
  EXPECT_EQ(m_queue.size(), 0);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 0);
  EXPECT_EQ(m_e3_history.size(), 0);
  EXPECT_EQ(m_id_history.size(), 0);

  m_queue.enqueue<E1>();
  m_queue.enqueue<E2>(1, 2);
  m_queue.enqueue<E3>("foo");

  EXPECT_EQ(m_queue.size(), 3);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 0);
  EXPECT_EQ(m_e3_history.size(), 0);
  EXPECT_EQ(m_id_history.size(), 0);

  m_queue.trigger<E2>(3, 4);

  EXPECT_EQ(m_queue.size(), 3);
  EXPECT_EQ(m_e1_history.size(), 0);
  EXPECT_EQ(m_e2_history.size(), 1);
  EXPECT_EQ(m_e3_history.size(), 0);

  ASSERT_EQ(m_id_history.size(), 1);
  EXPECT_EQ(m_id_history[0], E2::kID);

  m_queue.publish();

  EXPECT_EQ(m_queue.size(), 0);
  EXPECT_EQ(m_e1_history.size(), 1);
  EXPECT_EQ(m_e2_history.size(), 2);
  EXPECT_EQ(m_e3_history.size(), 1);

  ASSERT_EQ(m_id_history.size(), 4);
  EXPECT_EQ(m_id_history[0], E2::kID);
  EXPECT_EQ(m_id_history[1], E1::kID);
  EXPECT_EQ(m_id_history[2], E2::kID);
  EXPECT_EQ(m_id_history[3], E3::kID);
}

}  // namespace
}  // namespace tactile::editor
