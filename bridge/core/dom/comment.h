/*
 * Copyright (C) 2019-2022 The Kraken authors. All rights reserved.
 * Copyright (C) 2022-present The WebF authors. All rights reserved.
 */
#ifndef BRIDGE_COMMENT_H
#define BRIDGE_COMMENT_H

#include "character_data.h"

namespace webf {

class Comment : public CharacterData {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static Comment* Create(ExecutingContext* context, ExceptionState& exception_state);
  static Comment* Create(Document&);

  explicit Comment(TreeScope& tree_scope, ConstructionType type);

  NodeType nodeType() const override;

 private:
  std::string nodeName() const override;
  Node* Clone(Document&, CloneChildrenFlag) const override;
};

}  // namespace webf

#endif  // BRIDGE_COMMENT_H
