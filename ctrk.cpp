#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <map>

struct GameObject {
    uint32_t type;
    uint32_t group;
    float x, y, z;
};

// Returns a human-friendly name if known
std::string getTypeName(uint32_t type) {
    switch (type) {
        case 23: return "Cube";
        case 2147483648: return "Goofball";
        case 1065353216: return "Ball";
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
    std::map<uint32_t, int> typeCounts;

    char buffer[20];
    while (file.read(buffer, 20)) {
        GameObject obj;
        std::memcpy(&obj.type,  &buffer[0],  4);
        std::memcpy(&obj.group, &buffer[4],  4);
        std::memcpy(&obj.x,     &buffer[8],  4);
        std::memcpy(&obj.y,     &buffer[12], 4);
        std::memcpy(&obj.z,     &buffer[16], 4);
        objects.push_back(obj);
        typeCounts[obj.type]++;
    }

    std::cout << "[ctrk] Loaded " << objects.size() << " objects:\n\n";
    for (size_t i = 0; i < objects.size(); ++i) {
        const auto& o = objects[i];
        std::string name = getTypeName(o.type);
        std::cout << std::setw(3) << i
                  << ". Type ID: " << std::setw(3) << o.type
                  << " (" << std::setw(10) << name << ")"
                  << " | Pos: ("
                  << std::fixed << std::setprecision(2)
                  << o.x << ", " << o.y << ", " << o.z << ")\n";
    }

    std::cout << "\n📊 Type Frequency:\n";
    for (const auto& [type, count] : typeCounts) {
        std::cout << "  Type " << std::setw(3) << type
                  << " (" << getTypeName(type) << "): "
                  << count << " objects\n";
    }

    return 0;
}
