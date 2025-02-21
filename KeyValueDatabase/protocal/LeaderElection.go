func (rn *RaftNode) startElection() {
	rn.mu.Lock()
	rn.state = Candidate
	rn.currentTerm++
	rn.votedFor = rn.id
	rn.votes = 1 // Vote for itself
	fmt.Printf("Node %d starting election for term %d\n", rn.id, rn.currentTerm)
	rn.mu.Unlock()

	// Request votes from peers
	for _, peer := range rn.peers {
		go func(peer *RaftNode) {
			if peer.requestVote(rn.id, rn.currentTerm) {
				rn.mu.Lock()
				rn.votes++
				if rn.votes > len(rn.peers)/2 {
					rn.state = Leader
					fmt.Printf("Node %d became the leader for term %d\n", rn.id, rn.currentTerm)
					go rn.sendHeartbeats()
				}
				rn.mu.Unlock()
			}
		}(peer)
	}

	rn.resetElectionTimer()
}

func (rn *RaftNode) requestVote(candidateID, term int) bool {
	rn.mu.Lock()
	defer rn.mu.Unlock()

	if term > rn.currentTerm {
		rn.currentTerm = term
		rn.votedFor = candidateID
		fmt.Printf("Node %d voted for %d in term %d\n", rn.id, candidateID, term)
		return true
	}
	return false
}
