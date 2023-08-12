#include <stdio.h>
#include "std_msgs/msg/char.hpp"
#include <termios.h> 
#include <unistd.h>     
#include "rclcpp/rclcpp.hpp"

int getch()
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering      
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

  int c = getchar();  // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
  return c;
}

int main(int argc, char** argv){

  rclcpp::init(argc,argv);
  auto node = rclcpp::Node::make_shared("keyboard_driver");
  char key;
  auto char_pub_ = node->create_publisher<std_msgs::msg::Char>("/charstream", 10);
  
  while(rclcpp::ok()){
    // get the pressed key
    key = getch();
    
    std::printf("key is %c", key);
    
    auto c = std_msgs::msg::Char();
    c.data = key;

    char_pub_->publish(c);
  }
  return 0;

}
