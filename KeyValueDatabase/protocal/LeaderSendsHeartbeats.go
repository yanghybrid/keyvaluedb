func (rn *RaftNode) sendHeartbeats() {
	for rn.state == Leader {
		for _, peer := range rn.peers {
			go peer.receiveHeartbeat(rn.id, rn.currentTerm)
		}
		time.Sleep(2 * time.Second)
	}
}

func (rn *RaftNode) receiveHeartbeat(leaderID, term int) {
	rn.mu.Lock()
	defer rn.mu.Unlock()

	if term >= rn.currentTerm {
		rn.state = Follower
		rn.currentTerm = term
		rn.votedFor = leaderID
		rn.resetElectionTimer()
	}
}

func (rn *RaftNode) resetElectionTimer() {
	if rn.electionTimer != nil {
		rn.electionTimer.Stop()
	}
	timeout := time.Duration(rand.Intn(3000-1500)+1500) * time.Millisecond
	rn.electionTimer = time.AfterFunc(timeout, func() {
		if rn.state != Leader {
			rn.startElection()
		}
	})
}
