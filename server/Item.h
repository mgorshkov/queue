#pragma once

#include <queue>
#include <memory>
#include <string>

using Item = std::string;

using ItemPtr = std::unique_ptr<Item>;
using ItemQueue = std::queue<ItemPtr>;
