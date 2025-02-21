package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

// Raft Node States
const (
	Follower  = "Follower"
	Candidate = "Candidate"
	Leader    = "Leader"
)

// Raft Node Structure
type RaftNode struct {
	mu         sync.Mutex
	id         int
	state      string
	currentTerm int
	votedFor    int
	votes       int
	electionTimer *time.Timer
	peers      []*RaftNode
}

func NewRaftNode(id int) *RaftNode {
	node := &RaftNode{
		id:         id,
		state:      Follower,
		currentTerm: 0,
		votedFor:   -1,
		votes:      0,
	}
	node.resetElectionTimer()
	return node
}
