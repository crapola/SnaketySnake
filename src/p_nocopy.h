// Non-copyable macro.
#pragma once
#define NOCOPY(X) X(const X&)=delete;void operator=(const X&)=delete;
