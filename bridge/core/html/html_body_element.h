/*
 * Copyright (C) 2019-2022 The Kraken authors. All rights reserved.
 * Copyright (C) 2022-present The WebF authors. All rights reserved.
 */
#ifndef BRIDGE_CORE_HTML_HTML_BODY_ELEMENT_H_
#define BRIDGE_CORE_HTML_HTML_BODY_ELEMENT_H_

#include "html_element.h"

namespace webf {

class HTMLBodyElement : public HTMLElement {
  DEFINE_WRAPPERTYPEINFO();

 public:
  using ImplType = HTMLBodyElement*;
  explicit HTMLBodyElement(Document&);
};

}  // namespace webf

#endif  // BRIDGE_CORE_HTML_HTML_BODY_ELEMENT_H_