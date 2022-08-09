/*
 * Copyright (C) 2019-2022 The Kraken authors. All rights reserved.
 * Copyright (C) 2022-present The WebF authors. All rights reserved.
 */

#include "empty_node_list.h"
#include "core/dom/node.h"

namespace webf {

EmptyNodeList::EmptyNodeList(Node* root_node) : owner_(root_node), NodeList(root_node->ctx()) {}

void EmptyNodeList::Trace(GCVisitor* visitor) const {}

Node* EmptyNodeList::VirtualOwnerNode() const {
  return &OwnerNode();
}

}  // namespace webf