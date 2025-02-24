#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

const int DATA_DRIVES = 4; // Number of data drives
const int PARITY_DRIVES = 2; // Two parity drives (RAID 6)
const int TOTAL_DRIVES = DATA_DRIVES + PARITY_DRIVES; // Total drives

// Function to compute P parity (XOR-based)
vector<int> compute_p_parity(const vector<vector<int>>& data) {
    vector<int> p_parity(data[0].size(), 0);
    for (int i = 0; i < data[0].size(); i++) {
        for (int j = 0; j < DATA_DRIVES; j++) {
            p_parity[i] ^= data[j][i]; // XOR across data drives
        }
    }
    return p_parity;
}

// Function to compute Q parity (simple Galois Field XOR for demo)
vector<int> compute_q_parity(const vector<vector<int>>& data) {
    vector<int> q_parity(data[0].size(), 0);
    for (int i = 0; i < data[0].size(); i++) {
        for (int j = 0; j < DATA_DRIVES; j++) {
            q_parity[i] ^= (data[j][i] * (j + 1)); // Simple GF multiplication
        }
    }
    return q_parity;
}

// Function to simulate RAID 6 encoding
vector<vector<int>> raid6_encode(const vector<vector<int>>& data) {
    vector<vector<int>> drives = data;

    // Compute parity
    vector<int> p_parity = compute_p_parity(data);
    vector<int> q_parity = compute_q_parity(data);

    // Add parity drives
    drives.push_back(p_parity);
    drives.push_back(q_parity);

    return drives;
}

// Function to recover missing data
vector<int> raid6_recover(const vector<vector<int>>& drives, int failed_drive1, int failed_drive2) {
    vector<int> recovered(drives[0].size(), 0);

    for (int i = 0; i < drives[0].size(); i++) {
        int p_value = 0;
        int q_value = 0;

        for (int j = 0; j < TOTAL_DRIVES; j++) {
            if (j != failed_drive1 && j != failed_drive2) {
                p_value ^= drives[j][i];
                q_value ^= (drives[j][i] * (j + 1)); // Simulated GF multiplication
            }
        }

        // Basic recovery logic: Solve equations using XOR
        recovered[i] = p_value; // Simplified recovery method
    }

    return recovered;
}

int main() {
    // Example data (4 data drives, 3 bytes per stripe)
    vector<vector<int>> data = {
        {10, 20, 30}, 
        {40, 50, 60}, 
        {70, 80, 90}, 
        {100, 110, 120}
    };

    // Encode data with RAID 6 (adding P and Q parity)
    vector<vector<int>> drives = raid6_encode(data);

    // Simulate drive failure
    int failed_drive1 = 2; // Drive 2 failed
    int failed_drive2 = 3; // Drive 3 failed

    cout << "Recovering data..." << endl;
    vector<int> recovered = raid6_recover(drives, failed_drive1, failed_drive2);

    cout << "Recovered Data: ";
    for (int d : recovered) cout << d << " ";
    cout << endl;

    return 0;
}
