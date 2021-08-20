//
// Created by Ilya on 20.08.2021.
//

#include <gtest/gtest.h>
#include <Callback.h>


bool ConsistsOfDigits(const std::string &word)
{
  if (std::count_if(word.begin(), word.end(), [](const char &c){return std::isdigit(c);})
      == word.size()){
    return true;
  }
  return false;
}

bool ConsistsOfUpperCase(const std::string &word)
{
  if (std::count_if(word.begin(), word.end(), [](const char &c){return std::isupper(c);})
      == word.size()){
    return true;
  }
  return false;
}

TEST(CallbackTests, AddingPredicates){
  Callback callback;
  EXPECT_EQ(0, callback.GetCountPredicates());
  callback.ConnectWithPredicate(ConsistsOfUpperCase);
  EXPECT_EQ(1, callback.GetCountPredicates());
  callback.ConnectWithPredicate(ConsistsOfUpperCase);
  EXPECT_EQ(2, callback.GetCountPredicates());
}

TEST(CallbackTests, IsWorkingLikeConsistsOfDigits){
  Callback callback;
  callback.ConnectWithPredicate(ConsistsOfDigits);
  EXPECT_EQ(callback(std::string("123")), ConsistsOfDigits(std::string("123")));
  EXPECT_EQ(callback(std::string("123a")), ConsistsOfDigits(std::string("123a")));
  EXPECT_EQ(callback(std::string("aaa")), ConsistsOfDigits(std::string("aaa")));
  EXPECT_EQ(callback(std::string("AAA")), ConsistsOfDigits(std::string("AAA")));
}

TEST(CallbackTests, IsWorkingLikeConsistsOfUpperCase){
  Callback callback;
  callback.ConnectWithPredicate(ConsistsOfUpperCase);
  EXPECT_EQ(callback(std::string("123")), ConsistsOfUpperCase(std::string("123")));
  EXPECT_EQ(callback(std::string("123a")), ConsistsOfUpperCase(std::string("123a")));
  EXPECT_EQ(callback(std::string("aaa")), ConsistsOfUpperCase(std::string("aaa")));
  EXPECT_EQ(callback(std::string("AAA")), ConsistsOfUpperCase(std::string("AAA")));
}

TEST(CallbackTests, CorrectWorkingWithManyPredicates){
  Callback callback(ConsistsOfUpperCase);
  callback.ConnectWithPredicate(ConsistsOfDigits);
  EXPECT_EQ(callback(std::string("123")), true);
  EXPECT_EQ(callback(std::string("123a")), false);
  EXPECT_EQ(callback(std::string("AAA")), true);
  EXPECT_EQ(callback(std::string("12AS3")), false);
}