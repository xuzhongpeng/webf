/*
 * Copyright (C) 2019-2022 The Kraken authors. All rights reserved.
 * Copyright (C) 2022-present The WebF authors. All rights reserved.
 */
#include "script_state.h"
#include "defined_properties_initializer.h"
#include "event_factory.h"
#include "html_element_factory.h"
#include "names_installer.h"

namespace webf {

thread_local std::atomic<int32_t> runningContexts{0};

ScriptState::ScriptState(DartContext* dart_context) : dart_context_(dart_context) {
  runningContexts++;
  bool first_loaded = false;
  if (dart_context_->runtime() == nullptr) {
    dart_context_->InitializeJSRuntime();
    first_loaded = true;
  }
  // Avoid stack overflow when running in multiple threads.
  ctx_ = JS_NewContext(dart_context_->runtime());

  if (first_loaded) {
    names_installer::Init(ctx_);
    DefinedPropertiesInitializer::Init();
  }
}

JSRuntime* ScriptState::runtime() {
  return dart_context_->runtime();
}

ScriptState::~ScriptState() {
  ctx_invalid_ = true;
  JS_FreeContext(ctx_);

  // Run GC to clean up remaining objects about m_ctx;
  JS_RunGC(dart_context_->runtime());

  if (--runningContexts == 0) {
    dart_context_->DisposeJSRuntime();
  }
  ctx_ = nullptr;
}
}  // namespace webf
