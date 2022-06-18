#include "Utils.h"

bool Utils::IsPointInRect(const v2f& point, const v2f& pos, const v2f& size)
{
    return point.x > pos.x && point.x < pos.x + size.x
        && point.y > pos.y && point.y < pos.y + size.y;
}
