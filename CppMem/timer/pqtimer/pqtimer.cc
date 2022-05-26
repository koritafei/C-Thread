#include "pqtimer.h"

struct PQTimer::TimerNode {
  int           index   = -1;
  int           id      = -1;
  int64_t       expries = 0;
  TimerCallBack cb;
};

PQTimer::PQTimer() {
  _heap.reserve(64);
}

PQTimer::~PQTimer() {
  clear();
}

void PQTimer::clear() {
  _heap.clear();
}

#define HEAP_ITEM_LESS(i, j) (_heap[i]->expries < _heap[j]->expries)

bool PQTimer::siftdown(int x, int n) {
  int i = x;
  for (;;) {  // 循环交换根节点
    int j1 = i * 2 + 1;
    if (j1 >= n || j1 < 0) {
      break;
    }
    int j  = j1;  // 左孩子
    int j2 = j1 + 1;
    if (n > j2 && !HEAP_ITEM_LESS(j1, j2)) {  // 取左右孩子中较大的一个
      j = j2;                                 // 右孩子
    }
    if (!HEAP_ITEM_LESS(j, i)) {  // 孩子大于根节点，break
      break;
    }
    std::swap(_heap[i], _heap[j]);  // 否则交换较大的孩子与根节点
    _heap[i]->index = i;
    _heap[j]->index = j;
    i               = j;
  }
  return i > x;
}

void PQTimer::siftup(int j) {
  for (;;) {
    int i = (j - 1) / 2;  //父节点
    if (i == j || !HEAP_ITEM_LESS(j, i)) {
      break;
    }
    std::swap(_heap[i], _heap[j]);
    _heap[i]->index = i;
    _heap[j]->index = j;
    j               = i;
  }
}

/**
 * @brief
 *
 * @param tme_utils
 * @param cb
 * @return int
 */
int PQTimer::Schedule(uint32_t time_utils, TimerCallBack cb) {
  int64_t    exprise = Clock::CurrentTimeUnits() + time_utils;
  TimerNode *node    = new TimerNode;
  int        id      = nextCounter();
  node->id           = id;
  node->expries      = exprise;
  node->cb           = cb;
  node->index        = (int)_heap.size();
  _heap.push_back(node);
  siftup((int)_heap.size() - 1);
  _ref[id] = node;

  return id;
}

bool PQTimer::Cancel(int id) {
  auto iter = _ref.find(id);
  if (iter == _ref.end()) {
    return false;
  }
  Cancel(iter->second);
  return true;
}

void PQTimer::Cancel(TimerNode *node) {
  int n = (int)_heap.size() - 1;
  int i = node->index;
  if (i != n) {
    std::swap(_heap[i], _heap[n]);
    _heap[i]->index = i;
    if (!siftdown(i, n)) {
      siftup(i);
    }
  }
  _heap.pop_back();
  _ref.erase(node->id);
  delete node;
}

int PQTimer::Update(int64_t now) {
  if (_heap.empty()) {
    return 0;
  }
  if (0 == now) {
    now = Clock::CurrentTimeUnits();
  }
  int fired = 0;
  while (!_heap.empty()) {
    TimerNode *node = _heap.front();
    if (now < node->expries) {
      break;
    }
    auto cb = std::move(node->cb);
    Cancel(node);
    ++fired;
    if (cb) {
      cb();
    }
  }
  return fired;
}