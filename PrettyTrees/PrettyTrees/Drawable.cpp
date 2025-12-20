#include "Drawable.h"
#include "GlobalState.h"

Drawable::Vector2Int Drawable::GetScreenCoords() {
    return Vector2Int{
        // I'm not sure this was a good idea anymore but i'm too far along to revert back to using just normal pixel values
        // Maybe it would have been better if I actually had world transformations I just can't be bothered right now
        (int)(((pos.x * 0.5) + 0.5) * GlobalState::CANVAS_WIDTH),
        (int)((1.-((pos.y * 0.5) +0.5)) * GlobalState::CANVAS_HEIGHT)
    };
}
