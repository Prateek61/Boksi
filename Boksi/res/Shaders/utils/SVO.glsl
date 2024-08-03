struct Node
{
    uint8_t ChildrenMask;
    uint8_t Allignment[3]; // Hamro class 4 byte Allignment ma xa (at least in msvc) so 3 byte padding, dont use, could use to send materialID aswell in future 
    int Children[8]; // Index of the children node in the buffer
    uint8_t ChildrenVoxels[8];
}

// Sample layout for storage buffer
// layout(std430, binding=3) buffer Octrees {
//     Node nodes[];
// };