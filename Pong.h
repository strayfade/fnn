#include <vector>

class pongGame {

private:
    int paddleHeight = -1;
    int perfectPaddlePosition;

public:
    int score;
    int opponentScore;
    bool gameOver = false;

    enum class ballDirection {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    };
    struct ballData {
        int x = -1, y;
        ballDirection direction;  
    } ball;

    std::vector<float> getNetworkInputs();
    int aiPaddlePosition;
    void setAiPaddle(float y);

    void tick();
    void render();

};