#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int redraw_delay = 120;
long redraw_timer;
int graph_points[128];

/*
 * OLED and demo data setup
 */
void setup() {
  Serial.begin(9600);
  setup_display();
  randomSeed(collect_entropy());
  draw_current_frame();
}

void setup_display() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.cp437(true);
  display.dim(false);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}


/*
 * Utility functions for the demo
 */
long collect_entropy() {
  unsigned long acc = millis();
  for (int i = 0; i < 16; i++) {
    acc = (acc << 3) ^ ((map(analogRead(i), 256, 768, 0, 1024) * i) ^ (micros() >> 2));
  }
  return acc;
}

void fill_graph_points(int variance) {
  int last_value = random(0, 100);
  for (int i = 0; i < sizeof(graph_points)/sizeof(int); i++) {
    last_value = constrain((last_value + random(0, variance) - (variance/2)), 0, 100);
    graph_points[i] = last_value;
  }
}

void fill_graph_points() {
  fill_graph_points(random(1, 100));
}


/*
 * Redraw once per redraw_delay ms
 */
void loop() {
  render_tick();
}

void render_tick() {
  if (millis() - redraw_timer >= redraw_delay) {
    redraw_timer = millis();
    display.clearDisplay();
    draw_demo();
    display.display();
  }
}


/*
 * Current demo to draw
 *
 * Uncomment the one you want to see
 * Comment-out all the others (or else they'll overlap)
 */
void draw_demo() {
  demo_two_bar_graphs();
  // demo_bar_graph();
  // demo_autoscale_bar_graph();
  // demo_line_graph();
  // demo_area_graph();
  // demo_dot_graph();
  // demo_line_area_dot_graphs();
  // demo_linear_gauge();
  // demo_needle_meter();
  // demo_two_needle_meter();
  // demo_signal_strength();
  // demo_many_signal_strengths();
  // demo_thermometer();
  // demo_small_thermometers();
  // demo_segmented_gauge();
  // demo_dot_gauge();
  // demo_radial_gauge()
  // demo_radial_line_gauge();
  // demo_radial_segment_gauge();
  // demo_radial_dot_gauge();
}


/*
 * Demo functions
 */
void demo_two_bar_graphs() {
  int points_used = redraw_timer/redraw_delay%64;
  if (points_used < 1) fill_graph_points();

  draw_autoscale_bar_graph(0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, graph_points, points_used);
  draw_bar_graph(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, graph_points, points_used);
}

void demo_autoscale_bar_graph() {
  int points_used = redraw_timer/redraw_delay%128;
  if (points_used < 1) fill_graph_points();

  draw_autoscale_bar_graph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, graph_points, points_used);
}

void demo_bar_graph() {
  int points_used = redraw_timer/redraw_delay%32;
  if (points_used < 1) fill_graph_points();

  draw_bar_graph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, graph_points, points_used);
}

void demo_line_graph() {
  int points_used = redraw_timer/redraw_delay%128;
  if (points_used < 1) fill_graph_points();

  draw_line_graph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, graph_points, points_used);
}

void demo_area_graph() {
  int points_used = redraw_timer/redraw_delay%128;
  if (points_used < 1) fill_graph_points();

  draw_area_graph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, graph_points, points_used);
}

void demo_dot_graph() {
  int points_used = redraw_timer/redraw_delay%128;
  if (points_used < 1) fill_graph_points();

  draw_dot_graph(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, graph_points, points_used);
}

void demo_line_area_dot_graphs() {
  int points_used = redraw_timer/redraw_delay%64;
  if (points_used < 1) fill_graph_points();

  draw_line_graph(0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, graph_points, points_used);
  draw_area_graph(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, graph_points, points_used);
  draw_dot_graph(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, graph_points, points_used);
}

void demo_linear_gauge() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(5);

  draw_linear_gauge(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT/2, graph_points[current_point]);
  draw_linear_gauge(0, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, graph_points[current_point]);
  draw_linear_gauge(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 100-graph_points[current_point]);
}

void demo_needle_meter() {
  int current_point = redraw_timer/redraw_delay%128;
  if (current_point < 1) fill_graph_points(20);

  draw_needle_meter(0, 0, 128, graph_points[current_point]);
}

void demo_two_needle_meter() {
  int current_point = redraw_timer/redraw_delay%128;
  if (current_point < 1) fill_graph_points(20);

  draw_needle_meter(0, 0, 64, graph_points[current_point]);
  draw_needle_meter(64, 0, 64, graph_points[current_point]);
}

void demo_signal_strength() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(20);

  draw_signal_strength(0, 0, 128, 32, graph_points[current_point], 3);
}

void demo_many_signal_strengths() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(20);

  draw_signal_strength(0, 0, 64, 16, graph_points[current_point]);
  draw_signal_strength(0, 16, 64, 16, graph_points[current_point]);
  draw_signal_strength(64, 0, 32, 16, graph_points[current_point]);
  draw_signal_strength(64, 16, 32, 16, graph_points[current_point]);
  draw_signal_strength(96, 0, 32, 16, graph_points[current_point]);
  draw_signal_strength(96, 16, 32, 16, graph_points[current_point]);
}

void demo_thermometer() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(5);

  display.setRotation(1);
  draw_thermometer(0, 0, 31, 127, graph_points[current_point]);
}

void demo_small_thermometers() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(10);

  draw_thermometer(0, 0, 15, 32, graph_points[current_point]);
  draw_thermometer(16, 0, 15, 32, graph_points[current_point]);
  draw_thermometer(32, 0, 15, 32, graph_points[current_point]);
  draw_thermometer(48, 0, 15, 32, 100-graph_points[current_point]);
  draw_thermometer(64, 0, 7, 32, graph_points[current_point]);
  draw_thermometer(72, 0, 7, 32, graph_points[current_point]);
  draw_thermometer(80, 0, 7, 32, 100-graph_points[current_point]);
  draw_thermometer(88, 0, 7, 32, 100-graph_points[current_point]);
  draw_thermometer(96, 0, 15, 32, graph_points[current_point]);
  draw_thermometer(112, 0, 15, 32, 100-graph_points[current_point]);
}

void demo_segmented_gauge() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(25);

  draw_segmented_gauge(0, 0, 128, 12, graph_points[current_point], 12);
  draw_segmented_gauge(0, 16, 128, 12, graph_points[current_point], 12);
}

void demo_dot_gauge() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(25);

  draw_dot_gauge(0, 0, 128, 15, graph_points[current_point]);
  draw_dot_gauge(0, 16, 128, 7, 100-graph_points[current_point]);
  draw_dot_gauge(0, 24, 128, 7, graph_points[current_point]);
}

void demo_radial_gauge() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(30);

  draw_radial_gauge(15, 15, 15, graph_points[current_point]);
  draw_radial_gauge(47, 15, 15, 100-graph_points[current_point]);
  draw_radial_gauge(79, 15, 15, 100-graph_points[127-current_point]);
  draw_radial_gauge(111, 15, 15, graph_points[64-current_point]);
}

void demo_radial_dot_gauge() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(30);

  draw_radial_dot_gauge(15, 15, 12, 3, graph_points[current_point]);
  draw_radial_dot_gauge(47, 15, 12, 3, 100-graph_points[current_point], 10, 1, -PI/2);
  draw_radial_dot_gauge(79, 15, 12, 3, graph_points[100-current_point], 8, 3, -PI/2);
  draw_radial_dot_gauge(111, 15, 12, 3, 100-graph_points[100-current_point], 10, 3, -PI/2);
}

void demo_radial_segment_gauge() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(30);

  draw_radial_segment_gauge(15, 15, 15, 8, graph_points[current_point]);
  draw_radial_segment_gauge(47, 15, 15, 8, 100-graph_points[current_point]);
  draw_radial_segment_gauge(79, 15, 15, 6, graph_points[100-current_point]);
  draw_radial_segment_gauge(111, 15, 15, 10, 100-graph_points[100-current_point]);
}

void demo_radial_line_gauge() {
  int current_point = redraw_timer/redraw_delay%64;
  if (current_point < 1) fill_graph_points(30);

  draw_radial_line_gauge(15, 15, 15, 24, graph_points[current_point]);
  draw_radial_line_gauge(47, 15, 15, 24, 100-graph_points[current_point]);
  draw_radial_line_gauge(79, 15, 15, 24, graph_points[100-current_point]);
  draw_radial_line_gauge(111, 15, 15, 24, 100-graph_points[100-current_point]);
}
