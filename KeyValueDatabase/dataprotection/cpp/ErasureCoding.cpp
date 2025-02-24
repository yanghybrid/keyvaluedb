#include <iostream>
#include <vector>

using namespace std;

#define DATA_SHARDS 4
#define PARITY_SHARDS 2
#define TOTAL_SHARDS (DATA_SHARDS + PARITY_SHARDS)

// Simple XOR-based erasure coding (Reed-Solomon uses more complex Galois Field math)
vector<int> generate_parity(const vector<int>& data) {
    vector<int> parity(PARITY_SHARDS, 0);
    for (int d : data) {
        for (int i = 0; i < PARITY_SHARDS; i++) {
            parity[i] ^= d; // XOR parity
        }
    }
    return parity;
}

// Function to reconstruct lost data
vector<int> recover_data(const vector<int>& data, const vector<int>& parity, int lost_index) {
    vector<int> recovered_data = data;
    recovered_data[lost_index] = 0;

    for (int p : parity) {
        recovered_data[lost_index] ^= p; // XOR with parity
    }
    return recovered_data;
}

int main() {
    vector<int> data = {100, 200, 150, 250}; // Original Data
    vector<int> parity = generate_parity(data);

    // Simulate Data Loss (Lose index 2)
    data[2] = 0;
    cout << "Lost Data at index 2\n";

    vector<int> recovered = recover_data(data, parity, 2);
    
    cout << "Recovered Data: ";
    for (int d : recovered) cout << d << " ";
    cout << endl;

    return 0;
}
