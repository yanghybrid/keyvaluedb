func main() {
	// Create nodes
	nodes := []*RaftNode{}
	for i := 0; i < 5; i++ {
		nodes = append(nodes, NewRaftNode(i))
	}

	// Set peers
	for i := range nodes {
		nodes[i].peers = append(nodes[:i], nodes[i+1:]...)
	}

	// Run indefinitely
	select {}
}
