#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdint>
#include <cstring>

struct GameObject {
    uint32_t type;
    uint32_t group;
    float x, y, z;
};

std::string getTypeName(uint32_t type) {
    switch (type) {
        case 1: return "Goofball";
        case 2: return "Ball";
        case 3: return "Goal stand";
        case 4: return "Obstacle";
        default: return "Unknown";
    }
}

int main() {
    std::ifstream file("arena.trk", std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open arena.trk\n";
        return 1;
    }

    std::vector<GameObject> objects;
    char buffer[20];

    while (file.read(buffer, 20)) {
        GameObject obj;
        std::memcpy(&obj.type,  &buffer[0], 4);
        std::memcpy(&obj.group, &buffer[4], 4);
        std::memcpy(&obj.x,     &buffer[8], 4);
        std::memcpy(&obj.y,     &buffer[12], 4);
        std::memcpy(&obj.z,     &buffer[16], 4);
        objects.push_back(obj);
    }

    std::cout << "[ctrk] Loaded " << objects.size() << " objects:\n\n";
    for (size_t i = 0; i < objects.size(); ++i) {
        const auto& o = objects[i];
        std::cout << std::setw(3) << i << ". "
                  << std::setw(10) << getTypeName(o.type)
                  << " | Group: " << o.group
                  << " | Position: ("
                  << std::fixed << std::setprecision(2)
                  << o.x << ", " << o.y << ", " << o.z << ")\n";
    }

    return 0;
}
