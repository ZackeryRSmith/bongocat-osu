#include "header.hpp"

class OsuCatch : public Cat {
    public:
        Json::Value left_key_value, right_key_value, dash_key_value;
        sf::Sprite bg, mid, left, right, dash, up;

        int key_state = 0;
        bool left_key_state = false;
        bool right_key_state = false;
        double timer_left_key = -1;
        double timer_right_key = -1;

    OsuCatch() {
        // getting configs
        Json::Value osuCatch = data::cfg["cats"]["catch"];

        left_key_value = osuCatch["left"];
        right_key_value = osuCatch["right"];
        dash_key_value = osuCatch["dash"];
    
        // check for overlapping keybinds
        if (helpers::keys_overlapping({left_key_value, right_key_value, dash_key_value})) {
            data::error_msg("Overlapping osu! keybinds", "Error reading configs");
            exit(1);
        }

        /*
         * importing sprites
         */
        bg.setTexture(data::load_texture("cats/catch/bg.png"));
        mid.setTexture(data::load_texture("cats/catch/mid.png"));
        left.setTexture(data::load_texture("cats/catch/left.png"));
        right.setTexture(data::load_texture("cats/catch/right.png"));
        dash.setTexture(data::load_texture("cats/catch/dash.png"));
        up.setTexture(data::load_texture("cats/catch/up.png"));
    }

    void draw(const sf::RenderStates& rstates) override {
        bg.setPosition(0, window_height - bg.getTextureRect().height);
        window.draw(bg);
    
        // left key
        bool left_key = helpers::is_pressed(left_key_value);
    
        if (left_key) {
            if (!left_key_state) {
                key_state = 1;
                left_key_state = true;
            }
        } else {
        left_key_state = false;
        }

        // right key
        bool right_key = helpers::is_pressed(right_key_value);    

        if (right_key) {
            if (!right_key_state) {
                key_state = 2;
                right_key_state = true;
            }
        } else {
            right_key_state = false;
        }

        // dash
        bool is_dash = helpers::is_pressed(dash_key_value);
        window.draw(is_dash ? dash : up, rstates);


        /*
         * drawing mid points
         */
        if (!left_key_state && !right_key_state) {
            key_state = 0;
            window.draw(mid, rstates);
        }
        if (key_state == 1) {
            if ((clock() - timer_right_key) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(left, rstates);
                timer_left_key = clock();
            } else {
                window.draw(mid, rstates);
            }
        } else if (key_state == 2) {
            if ((clock() - timer_left_key) / CLOCKS_PER_SEC > BONGO_KEYPRESS_THRESHOLD) {
                window.draw(right, rstates);
                timer_right_key = clock();
            } else {
                window.draw(mid, rstates);
            }
        }
    }
}; // namespace osuCatch
