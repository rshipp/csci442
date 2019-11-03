/**
 * This file contains tests for the methods declared in formatting.h.
 */

#include "formatting.h"
#include "gtest/gtest.h"

using namespace std;


TEST(HumanizeBytes, B) {
  ASSERT_EQ("1B", humanize_bytes(1));
  ASSERT_EQ("1023B", humanize_bytes(1023));
}

TEST(HumanizeBytes, KiB) {
  ASSERT_EQ("1KiB", humanize_bytes(1024));
  ASSERT_EQ("1023KiB", humanize_bytes(1024*1024-1));
}

TEST(HumanizeBytes, MiB) {
  ASSERT_EQ("1MiB", humanize_bytes(1024*1024));
  ASSERT_EQ("1023MiB", humanize_bytes(1024*1024*1024-1));
}

TEST(HumanizeBytes, GiB) {
  ASSERT_EQ("1GiB", humanize_bytes(1024*1024*1024));
  ASSERT_EQ("1024GiB", humanize_bytes(1024*1024*1024*1024L));
}

TEST(HumanizeTime, AM) {
}

TEST(HumanizeTime, PM) {
}

TEST(HumanizeUptime, SS) {
  ASSERT_EQ("00:00:01", humanize_uptime(1));
  ASSERT_EQ("00:00:59", humanize_uptime(59));
}

TEST(HumanizeUptime, MM) {
  ASSERT_EQ("00:01:00", humanize_uptime(60));
  ASSERT_EQ("00:01:01", humanize_uptime(61));
  ASSERT_EQ("00:01:59", humanize_uptime(60+59));
  ASSERT_EQ("00:10:00", humanize_uptime(60*10));
  ASSERT_EQ("00:10:01", humanize_uptime(60*10+1));
  ASSERT_EQ("00:10:59", humanize_uptime(60*10+59));
  ASSERT_EQ("00:59:59", humanize_uptime(60*60-1));
}

TEST(HumanizeUptime, HH) {
  ASSERT_EQ("01:00:00", humanize_uptime(60*60));
  ASSERT_EQ("01:00:01", humanize_uptime(60*60+1));
  ASSERT_EQ("01:01:00", humanize_uptime(60*61));
  ASSERT_EQ("01:01:01", humanize_uptime(60*61+1));
  ASSERT_EQ("01:59:59", humanize_uptime(60*(60+59)+59));
  ASSERT_EQ("23:59:59", humanize_uptime(60*60*24-1));
}

TEST(HumanizeUptime, Days) {
  ASSERT_EQ("1 days, 00:00:00", humanize_uptime(60*60*24));
  ASSERT_EQ("1 days, 00:00:01", humanize_uptime(60*60*24+1));
  ASSERT_EQ("1 days, 01:00:00", humanize_uptime((60*60*24)+60*60));
  ASSERT_EQ("1 days, 01:00:01", humanize_uptime((60*60*24)+60*60+1));
  ASSERT_EQ("1 days, 01:01:00", humanize_uptime((60*60*24)+60*61));
  ASSERT_EQ("1 days, 01:01:01", humanize_uptime((60*60*24)+60*61+1));
  ASSERT_EQ("1 days, 01:59:59", humanize_uptime((60*60*24)+60*(60+59)+59));
  ASSERT_EQ("1 days, 23:59:59", humanize_uptime((60*60*24)+60*60*24-1));
  ASSERT_EQ("2 days, 00:00:00", humanize_uptime((60*60*24)*2));
}
