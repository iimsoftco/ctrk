#include "stuff/Buffer.h"
#include "stuff/FileLoader.h"

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <vector>
#include <map>

struct GameObject {
    uint32_t type;
    uint32_t group;
    float x, y, z;
};

std::string getTypeName(uint32_t type) {
    switch (type) {
        case 23: return "Cube";
        case 2147483648: return "Goofball (Blue)";
        case 3: return "Goofball (Red)";
        case 1065353216: return "Ball";
        case 5: return "Gate";
        default: return "Unknown";
    }
}

int main() {
    try {
        Buffer buffer = fileLoader::Load("arena.trk");

        

        std::vector<GameObject> objects;
        std::map<uint32_t, int> typeCounts;

        size_t offset = 0;
        while (offset + 20 <= buffer.size()) {
            GameObject obj;
            std::memcpy(&obj.type,  buffer.data() + offset + 0, 4);
            std::memcpy(&obj.group, buffer.data() + offset + 4, 4);
            std::memcpy(&obj.x,     buffer.data() + offset + 8, 4);
            std::memcpy(&obj.y,     buffer.data() + offset + 12, 4);
            std::memcpy(&obj.z,     buffer.data() + offset + 16, 4);

            objects.push_back(obj);
            typeCounts[obj.type]++;
            offset += 20;
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

        std::cout << "\n[ctrk] Type Frequency:\n";
        for (const auto& [type, count] : typeCounts) {
            std::cout << "  Type " << std::setw(3) << type
                      << " (" << getTypeName(type) << "): "
                      << count << " objects\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "[ctrk] Error loading file: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
