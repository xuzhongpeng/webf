/*
* Copyright (C) 2019-2022 The Kraken authors. All rights reserved.
* Copyright (C) 2022-present The WebF authors. All rights reserved.
*/

#include "widget_element.h"
#include "foundation/native_value_converter.h"

namespace webf {

WidgetElement::WidgetElement(const AtomicString& tag_name, Document* document): HTMLElement(tag_name, document, ConstructionType::kCreateWidgetElement) {}

bool WidgetElement::NamedPropertyQuery(const AtomicString& key, ExceptionState& exception_state) {
  NativeValue result = GetBindingProperty(key, exception_state);
  return result.tag != NativeTag::TAG_NULL;
}

void WidgetElement::NamedPropertyEnumerator(std::vector<AtomicString>& names, ExceptionState& exception_state) {
  NativeValue result = GetAllBindingPropertyNames(exception_state);
  assert(result.tag == NativeTag::TAG_LIST);
  std::vector<AtomicString> property_names = NativeValueConverter<NativeTypeArray<NativeTypeString>>::FromNativeValue(ctx(), result);
  names.reserve(property_names.size());
  for(int i = 0; i < property_names.size(); i ++) {
    names[i] = property_names[i];
  }
}

ScriptValue WidgetElement::item(const AtomicString& key, ExceptionState& exception_state) {
  return ScriptValue(ctx(), GetBindingProperty(key, exception_state));
}

bool WidgetElement::SetItem(const AtomicString& key, const ScriptValue& value, ExceptionState& exception_state) {
  NativeValue result = SetBindingProperty(key, value.ToNative(), exception_state);
  return NativeValueConverter<NativeTypeBool>::FromNativeValue(result);
}

}