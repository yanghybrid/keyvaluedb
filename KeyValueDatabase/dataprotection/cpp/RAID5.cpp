#include <iostream>
#include <vector>

using namespace std;

// Function to simulate RAID 5 striping and parity generation
vector<vector<int>> raid5_encode(const vector<int>& data, int num_drives) {
    int stripe_size = (data.size() + num_drives - 2) / (num_drives - 1);
    vector<vector<int>> drives(num_drives, vector<int>(stripe_size, 0));

    for (int i = 0, idx = 0; i < stripe_size; i++) {
        int parity = 0, parity_index = i % num_drives;
        
        for (int j = 0; j < num_drives; j++) {
            if (j == parity_index) continue; // Parity block position
            if (idx < data.size()) {
                drives[j][i] = data[idx++];
                parity ^= drives[j][i];
            }
        }
        drives[parity_index][i] = parity; // Store parity
    }
    return drives;
}

// Function to recover a missing drive in RAID 5
vector<int> raid5_recover(const vector<vector<int>>& drives, int failed_drive) {
    vector<int> recovered_data;

    for (int i = 0; i < drives[0].size(); i++) {
        int parity = 0, recovered_value = 0;

        for (int j = 0; j < drives.size(); j++) {
            if (j != failed_drive) {
                recovered_value ^= drives[j][i];
            } else {
                parity = drives[j][i]; // The missing parity or data block
            }
        }
        recovered_data.push_back(recovered_value ^ parity);
    }
    return recovered_data;
}

int main() {
    vector<int> data = {10, 20, 30, 40, 50, 60, 70}; // Example data
    int num_drives = 4; // RAID 5 with 3 data + 1 parity

    vector<vector<int>> drives = raid5_encode(data, num_drives);

    // Simulate a drive failure (e.g., Drive 2 fails)
    int failed_drive = 2;
    vector<int> recovered = raid5_recover(drives, failed_drive);

    cout << "Recovered Data: ";
    for (int d : recovered) cout << d << " ";
    cout << endl;

    return 0;
}
