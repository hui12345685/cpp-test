/*
  Copyright 2018

  Exception for future, using member function is(ExceptionType) to
  distinguish exception kind instead of throwing and cathing. But it's not
  friendly for coroutine.
*/

#pragma once

#include <memory>
#include <string>
#include <type_traits>

/**
 * @brief Exception base class.
 * Derived classed can override @ref what function to return a customize error
 * string.
 */
class ExceptionBase {
 public:
  virtual const char *what() const noexcept = 0;
};

class CommonException : public ExceptionBase {
 public:
  CommonException(const char *msg) { msg_ = std::string(msg); }
  const char *what() const noexcept override { return msg_.c_str(); }

 private:
  std::string msg_;
};

/**
 * @brief ExceptionBase wrapper.
 * Any derived exception class can cast to Exception.
 * Support get error message and check exception type in runtime.
 */
class Exception final {
 public:
  Exception() = default;
  ~Exception() = default;
  Exception(Exception &&e) = default;
  Exception(const Exception &e) = default;
  Exception &operator=(Exception &&e) = default;
  Exception &operator=(const Exception &e) = default;
  template <typename ExceptionType,
            typename = typename std::enable_if<std::is_base_of<ExceptionBase, ExceptionType>::value>::type>
  Exception &operator=(ExceptionType e) {
    ptr = std::make_shared<ExceptionType>(e);
    return *this;
  }
  template <typename ExceptionType,
            typename = typename std::enable_if<std::is_base_of<ExceptionBase, ExceptionType>::value>::type>
  Exception(ExceptionType e) {
    ptr = std::make_shared<ExceptionType>(e);
  }

  const char *what() const {
    if (ptr == nullptr) {
      return "empty exception";
    }
    return ptr->what();
  }

  /**
   * @brief Check the exception object is ExceptionType or not.
   *
   * @tparam ExceptionType
   * @tparam std::enable_if<std::is_base_of<ExceptionBase,
   * ExceptionType>::value>::type
   * @return true If the exception object is ExceptionType or derived from
   * ExceptionType.
   * @return false Otherwise.
   */
  template <typename ExceptionType,
            typename = typename std::enable_if<std::is_base_of<ExceptionBase, ExceptionType>::value>::type>
  bool is(const ExceptionType &) const {
    if (ptr == nullptr) {
      return false;
    }
    if (dynamic_cast<ExceptionType *>(ptr.get()) != nullptr) {
      return true;
    }
    return false;
  }

 private:
  std::shared_ptr<ExceptionBase> ptr = nullptr;
};
