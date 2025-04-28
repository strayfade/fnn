#include <vector>

class pongGame {

private:
    int paddleHeight = -1;

public:
    int score = 0;
    int opponentScore = 0;
    bool gameOver = false;

    struct rendererSize_t {
        int ws_row = 0;
        int ws_col = 0;
    } cachedWindowSize;
    rendererSize_t getRendererSize();

    enum class ballDirection {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    };
    struct ballData {
        int x = -1, y;
        ballDirection direction= ballDirection::TOP_LEFT;
    } ball;

    int aiPaddlePosition;
    int perfectPaddlePosition;
    void setAiPaddle(float y);
    void setOpponentPaddle(float y);

    void tick();
    void render();

};