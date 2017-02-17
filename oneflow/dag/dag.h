#ifndef ONEFLOW_DAG_DAG_H_
#define ONEFLOW_DAG_DAG_H_

#include <iostream>
#include <queue>
#include "dag/dag_node.h"

namespace oneflow {

class Dag {
 public:
  // Topologically ergodic all nodes except start_node_,stop_node_
  class DagIterator {
   public:
    // DISALLOW_MOVE(DagIterator);
    DagIterator(const DagIterator&);
    DagIterator& operator = (const DagIterator&);
    
    DagIterator() = default;
    ~DagIterator() = default;
    
    void init(DagNode* start_node) {
      bfs_queue_ = std::make_shared<std::queue<DagNode*>> ();
      bfs_queue_->push(start_node);
    }
    
    DagNode& operator * ();
    DagNode* operator -> ();
    void operator ++ ();
    
    bool operator != (const DagIterator&) const;

   private:
    // we need to make light-object
    std::shared_ptr<std::queue<DagNode*>> bfs_queue_;
  };
  class ConstDagIterator {
   public:
    // DISALLOW_COPY_AND_MOVE(ConstDagIterator);
    ConstDagIterator() = default;
    ~ConstDagIterator() = default;
    
    void init(DagIterator dag_iterator) {
      dag_iterator_ = dag_iterator;
    }
    
    const DagNode& operator * () { return *dag_iterator_; }
    const DagNode* operator -> () { return &(*dag_iterator_); }
    void operator ++ () { ++dag_iterator_; }
    bool operator != (const ConstDagIterator& rhs) const {
      return dag_iterator_ != rhs.dag_iterator_;
    }

   private:
    DagIterator dag_iterator_;
  };

  DISALLOW_COPY_AND_MOVE(Dag);
  Dag() = default;
  virtual ~Dag() = default;

  void init(const std::string& dag_name) {
    dag_name_ = dag_name;
    start_node_.init();
    stop_node_.init();
  }

  const std::string& dag_name() { return dag_name_; }
  const DagNode& start_node() const { return start_node_; }
  const DagNode& stop_node() const { return stop_node_; }

  DagIterator begin() {
    DagIterator ret;
    ret.init(&start_node_);
    ++ret;
    return ret;
  }
  DagIterator end() {
    DagIterator ret;
    ret.init(&stop_node_);
    return ret;
  }

  ConstDagIterator cbegin() const {
    ConstDagIterator ret;
    ret.init((const_cast<Dag*>(this))->begin());
  }
  ConstDagIterator cend() const {
    ConstDagIterator ret;
    ret.init((const_cast<Dag*>(this))->end());
  }

 private:
  std::string dag_name_;
  DagNode start_node_;
  DagNode stop_node_;

};

} // namespace oneflow
#endif // ONEFLOW_DAG_DAG_H_
