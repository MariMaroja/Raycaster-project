#include <vector>
#include <SFML/Graphics.hpp>
#include <optional>
#include <cmath>

// Simple map:
// 1 = wall
// 0 = empty space
const std::vector<std::vector<int>> worldMap = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};


int main(){
    // Create the game window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML works!");
    // Hide the mouse cursor for FPS-style controls
    window.setMouseCursorVisible(false);
    // Player starting position
    sf::Vector2f playerPos = {75.f, 75.f};
    // Player movement speed
    float playerSpeed = 3.f;
    // Player viewing angle (in radians)
    float playerAngle = 0.f;
    // Toggle minimap visibility
    bool showMap = true;
    while(window.isOpen()){
        // Event handling
        while(const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            // Toggle map when TAB is pressed
            if(event->is<sf::Event::KeyPressed>()){
                const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
                if (keyPressed && keyPressed->code == sf::Keyboard::Key::Tab){
                    showMap = !showMap;
                }
            }
        }
        const float tileSize = 50.f;
        // Temporary position used for collision checking
        sf::Vector2f newPos = { playerPos.x, playerPos.y};
        // Move forward
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
            newPos.x += std::cos(playerAngle) * playerSpeed;
            newPos.y += std::sin(playerAngle) * playerSpeed;
        }
        // Move backward
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
            newPos.x -= std::cos(playerAngle) * playerSpeed;
            newPos.y -= std::sin(playerAngle) * playerSpeed;
        }
        // Rotate left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
            playerAngle -= 0.05f;
        }
        // Rotate right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
            playerAngle += 0.05f;
        }
        // Mouse-look system
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int centerX = 400;
        float mouseSensitivity = 0.002f;
        // Update view angle based on mouse movement
        playerAngle += (mousePos.x - centerX) * mouseSensitivity;
        // Re-center mouse every frame
        sf::Mouse::setPosition(sf::Vector2i(centerX, 300), window);
        // Collision detection
        int mapX = static_cast<int>(newPos.x / tileSize);
        int mapY = static_cast<int>(newPos.y / tileSize);
        if (mapY >= 0 && mapY <worldMap.size() && mapX >= 0 && mapX < worldMap[0].size()){
            // Only move if the destination tile is empty
            if (worldMap[mapY][mapX] == 0){
                playerPos = newPos;
            }
        }
        window.clear();
        // Draw ceiling
        sf::RectangleShape ceiling;
        ceiling.setSize(sf::Vector2f(400.f, 300.f));
        ceiling.setFillColor(sf::Color(80, 80, 80));
        ceiling.setPosition({400.f, 0.f});
        window.draw(ceiling);
        // Draw floor
        sf::RectangleShape floor;
        floor.setSize(sf::Vector2f(400.f, 300.f));
        floor.setFillColor(sf::Color(40, 40, 40));
        floor.setPosition({400.f, 300.f});
        window.draw(floor);
        // Draw minimap
        if (showMap){
            for (size_t y = 0; y < worldMap.size(); y++){
                for (size_t x = 0; x < worldMap[y].size(); x++){
                    sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                    // Scale minimap to 50%
                    tile.setPosition({x * tileSize * 0.5f, y * tileSize * 0.5f});
                    // White = wall, Black = empty space
                    if (worldMap[y][x] == 1){
                    tile.setFillColor(sf::Color::White);
                } else {
                    tile.setFillColor(sf::Color::Black);
                }
                window.draw(tile);
            }
        }
        // Draw player on minimap
        sf::CircleShape player(8.f);
        player.setFillColor(sf::Color::Red);
        player.setPosition(sf::Vector2f(playerPos.x * 0.5f, playerPos.y * 0.5f));
        window.draw(player);
        sf::Vertex line[] = {
            sf::Vertex{playerPos + sf::Vector2f(8.f, 8.f), sf::Color::Red},
            sf::Vertex{playerPos + sf::Vector2f(8.f + std::cos(playerAngle) * 40.f, 8.f + std::sin(playerAngle) * 40.f)},
            sf::Vertex{playerPos + sf::Vector2f(8.f, 8.f), sf::Color::Yellow},
        };
        // Field of View (in radians)
        float fov = 1.0f;
        // Width of each wall slice in 3D view
        float wallWidth = 400.f / 60.f;
        // Raycasting loop
        for (int ray = 0; ray < 120; ray++){
            sf::Vector2f playerCenter = playerPos + sf::Vector2f(8.f, 8.f);
            // Calculate ray angle
            float rayAngle = playerAngle - (fov / 2.f) + ((ray / 120.f) * fov);
            sf::Vector2f rayEnd = playerPos;
            // Cast ray forward until it hits a wall
            for (int i = 0; i < 200; i++){
                rayEnd.x += std::cos(rayAngle);
                rayEnd.y += std::sin(rayAngle);
                int rayMapX = static_cast<int>(rayEnd.x / tileSize);
                int rayMapY = static_cast<int>(rayEnd.y / tileSize);
                if (rayMapY >= 0 && rayMapY <worldMap.size() && rayMapX >= 0 && rayMapX < worldMap[0].size()){
                    if (worldMap[rayMapY][rayMapX] == 1){
                        break;
                    }
                }
            }
            // Distance from player to wall
            float rayDistance = std::sqrt(std::pow(rayEnd.x - playerPos.x, 2) + std::pow(rayEnd.y - playerPos.y, 2));
            // Fish-eye correction
            rayDistance *= std::cos(rayAngle - playerAngle);
            float correctedDistance = rayDistance * std::cos(rayAngle - playerAngle);
            // Calculate wall height
            float wallHeight = 10000.f / rayDistance;
            sf::RectangleShape wall;
            wall.setSize(sf::Vector2f(wallWidth, wallHeight));
            // Distance-based shading
            float brightness = std::max(0.f, 255.f - (rayDistance * 2.f));
            wall.setFillColor(sf::Color(0, 0, static_cast<std::uint8_t>(brightness)));
            // Position wall slice in 3D viewport
            wall.setPosition(sf::Vector2f(400.f + (ray * wallWidth), 300.f - (wallHeight / 2.f)));
            if (brightness < 40.f){
                brightness = 40.f;
            }
            // Draw ray on minimap
            sf::Vertex rayLine[] = {
                sf::Vertex{
                    playerPos + sf::Vector2f(8.f, 8.f),
                    sf::Color::Green
                },
                sf::Vertex{
                    sf::Vector2f(rayEnd.x * 0.5f + 8.f, rayEnd.y * 0.5f + 8.f),
                    sf::Color::Green
                }
            };
            window.draw(rayLine, 2, sf::PrimitiveType::Lines);
            float wallWidth = 400.f / 120.f;
            
        }}
        // Draw player direction line
        sf::Vertex line[] = {
            sf::Vertex{playerPos + sf::Vector2f(8.f, 8.f), sf::Color::Red},
            sf::Vertex{playerPos + sf::Vector2f(8.f + std::cos(playerAngle) * 40.f, 8.f + std::sin(playerAngle) * 40.f)},
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
        
        window.display();
    }
    return 0;
}