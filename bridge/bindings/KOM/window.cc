/*
 * Copyright (C) 2019 Alibaba Inc. All rights reserved.
 * Author: Kraken Team.
 */

#include "window.h"
#include "dart_methods.h"
#include <cassert>

namespace kraken {
namespace binding {

using namespace alibaba::jsa;

Value JSWindow::get(JSContext &context, const PropNameID &name) {
  auto _name = name.utf8(context);
  if (_name == "devicePixelRatio") {
    if (getDartMethod()->devicePixelRatio == nullptr) {
      throw JSError(context, "Failed to read devicePixelRatio: dart method (devicePixelRatio) is not register.");
    }

    double devicePixelRatio = getDartMethod()->devicePixelRatio();
    return Value(devicePixelRatio);
  } else if (_name == "colorScheme") {
    if (getDartMethod()->platformBrightness == nullptr) {
      throw JSError(context, "Failed to read colorScheme: dart method (platformBrightness) not register.");
    }

    return String::createFromUtf8(context, getDartMethod()->platformBrightness());
  } else if (_name == "location") {
    return Value(context, Object::createFromHostObject(context, location_->shared_from_this()));
  }

  return Value::undefined();
}

void JSWindow::set(JSContext &context, const PropNameID &name, const Value &value) {}

void JSWindow::bind(std::unique_ptr<JSContext> &context) {
  assert(context != nullptr);

  Object &&window = Object::createFromHostObject(*context, sharedSelf());
  location_->bind(context, window);
  JSA_SET_PROPERTY(*context, context->global(), "__kraken_window__", window);
}

void JSWindow::unbind(std::unique_ptr<JSContext> &context) {
  Value &&window = JSA_GET_PROPERTY(*context, context->global(), "__kraken_window__");
  Object &&object = window.getObject(*context);
  location_->unbind(context, object);
  JSA_SET_PROPERTY(*context, context->global(), "__kraken_window__", Value::undefined());
}

std::vector<PropNameID> JSWindow::getPropertyNames(JSContext &context) {
  std::vector<PropNameID> names;
  names.emplace_back(PropNameID::forUtf8(context, "colorScheme"));
  names.emplace_back(PropNameID::forUtf8(context, "devicePixelRatio"));
  names.emplace_back(PropNameID::forUtf8(context, "location"));
  return names;
}

} // namespace binding
} // namespace kraken
