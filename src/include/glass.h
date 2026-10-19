#include "geometry.h"

class Glass:public Geometry
{
public:
    Glass(const std::vector<Vertex>& vertices);
    Glass(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);
    void Draw()override;
};