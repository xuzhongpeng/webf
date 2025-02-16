/*
 * Copyright (C) 2019-2022 The Kraken authors. All rights reserved.
 * Copyright (C) 2022-present The WebF authors. All rights reserved.
 */
#include "window_or_worker_global_scope.h"
#include "core/frame/dom_timer.h"

namespace webf {

static void handleTimerCallback(DOMTimer* timer, const char* errmsg) {
  auto* context = timer->context();

  if (errmsg != nullptr) {
    JSValue exception = JS_ThrowTypeError(context->ctx(), "%s", errmsg);
    context->HandleException(&exception);
    context->Timers()->forceStopTimeoutById(timer->timerId());
    return;
  }

  if (context->Timers()->getTimerById(timer->timerId()) == nullptr)
    return;

  // Trigger timer callbacks.
  timer->Fire();
}

static void handleTransientCallback(void* ptr, int32_t contextId, const char* errmsg) {
  auto* timer = static_cast<DOMTimer*>(ptr);
  auto* context = timer->context();

  if (!context->IsContextValid())
    return;

  if (timer->status() == DOMTimer::TimerStatus::kCanceled) {
    return;
  }

  timer->SetStatus(DOMTimer::TimerStatus::kExecuting);
  handleTimerCallback(timer, errmsg);
  timer->SetStatus(DOMTimer::TimerStatus::kFinished);

  context->Timers()->removeTimeoutById(timer->timerId());
}

static void handlePersistentCallback(void* ptr, int32_t contextId, const char* errmsg) {
  auto* timer = static_cast<DOMTimer*>(ptr);
  auto* context = timer->context();

  if (!context->IsContextValid())
    return;

  if (timer->status() == DOMTimer::TimerStatus::kCanceled) {
    return;
  }

  handleTimerCallback(timer, errmsg);
}

int WindowOrWorkerGlobalScope::setTimeout(ExecutingContext* context,
                                          std::shared_ptr<QJSFunction> handler,
                                          ExceptionState& exception) {
  return setTimeout(context, handler, 0.0, exception);
}

int WindowOrWorkerGlobalScope::setTimeout(ExecutingContext* context,
                                          std::shared_ptr<QJSFunction> handler,
                                          int32_t timeout,
                                          ExceptionState& exception) {
#if FLUTTER_BACKEND
  if (context->dartMethodPtr()->setTimeout == nullptr) {
    exception.ThrowException(context->ctx(), ErrorType::InternalError,
                             "Failed to execute 'setTimeout': dart method (setTimeout) is not registered.");
    return -1;
  }
#endif

  // Create a timer object to keep track timer callback.
  auto timer = DOMTimer::create(context, handler, DOMTimer::TimerKind::kOnce);
  auto timerId =
      context->dartMethodPtr()->setTimeout(timer.get(), context->contextId(), handleTransientCallback, timeout);

  // Register timerId.
  timer->setTimerId(timerId);

  context->Timers()->installNewTimer(context, timerId, timer);

  return timerId;
}

int WindowOrWorkerGlobalScope::setInterval(ExecutingContext* context,
                                           std::shared_ptr<QJSFunction> handler,
                                           ExceptionState& exception) {
  return setInterval(context, handler, 0.0, exception);
}

int WindowOrWorkerGlobalScope::setInterval(ExecutingContext* context,
                                           std::shared_ptr<QJSFunction> handler,
                                           int32_t timeout,
                                           ExceptionState& exception) {
  if (context->dartMethodPtr()->setInterval == nullptr) {
    exception.ThrowException(context->ctx(), ErrorType::InternalError,
                             "Failed to execute 'setInterval': dart method (setInterval) is not registered.");
    return -1;
  }

  // Create a timer object to keep track timer callback.
  auto timer = DOMTimer::create(context, handler, DOMTimer::TimerKind::kMultiple);

  uint32_t timerId =
      context->dartMethodPtr()->setInterval(timer.get(), context->contextId(), handlePersistentCallback, timeout);

  // Register timerId.
  timer->setTimerId(timerId);
  context->Timers()->installNewTimer(context, timerId, timer);

  return timerId;
}

void WindowOrWorkerGlobalScope::clearTimeout(ExecutingContext* context, int32_t timerId, ExceptionState& exception) {
  if (context->dartMethodPtr()->clearTimeout == nullptr) {
    exception.ThrowException(context->ctx(), ErrorType::InternalError,
                             "Failed to execute 'clearTimeout': dart method (clearTimeout) is not registered.");
    return;
  }

  context->dartMethodPtr()->clearTimeout(context->contextId(), timerId);
  context->Timers()->forceStopTimeoutById(timerId);
}

void WindowOrWorkerGlobalScope::clearInterval(ExecutingContext* context, int32_t timerId, ExceptionState& exception) {
  if (context->dartMethodPtr()->clearTimeout == nullptr) {
    exception.ThrowException(context->ctx(), ErrorType::InternalError,
                             "Failed to execute 'clearTimeout': dart method (clearTimeout) is not registered.");
    return;
  }

  context->dartMethodPtr()->clearTimeout(context->contextId(), timerId);
  context->Timers()->forceStopTimeoutById(timerId);
}

}  // namespace webf
