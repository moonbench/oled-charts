# OLED-Charts
Library of graphing tools for SSD1306 OLED displays

## About
Todo

## How to Demo
Todo

## Available Charts & Graphs
### Graphs
```c++
draw_line_graph(int x, int y, int width, int height, int data[], int data_count, bool line, bool fill);

draw_line_graph(int x, int y, int width, int height, int data[], int data_count);

draw_area_graph(int x, int y, int width, int height, int data[], int data_count);

draw_dot_graph(int x, int y, int width, int height, int data[], int data_count);

draw_bar_graph(int x, int y, int width, int height, int data[], int data_count, int bar_width, int bar_padding);

draw_bar_graph(int x, int y, int width, int height, int data[], int data_count);

draw_autoscale_bar_graph(int x, int y, int width, int height, int data[], int data_count);
```

### Gauges
```c++
draw_linear_gauge(int x, int y, int width, int height, int value);

draw_needle_meter(int x, int y, int width, int value);

draw_segmented_gauge(int x, int y, int width, int height, int value, int segments);

draw_dot_gauge(int x, int y, int width, int height, int value);

draw_radial_gauge(int x, int y, int radius, int value, int padding, bool outer_border, bool inner_border, bool draw_line, float start_offset);

draw_radial_gauge(int x, int y, int radius, int value);

draw_radial_gauge(int x, int y, int radius, int value, int padding);

draw_radial_dot_gauge(int x, int y, int radius, int dot_radius, int value, int segments, int empty_dot_radius, float start_offset);

draw_radial_dot_gauge(int x, int y, int radius, int dot_radius, int value);

draw_radial_segment_gauge(int x, int y, int radius, int segments, int value, int padding, bool outer_border, bool inner_border, float start_offset);

draw_radial_segment_gauge(int x, int y, int radius, int segments, int value);

draw_radial_line_gauge(int x, int y, int radius, int lines, int value, bool outer_border, bool inner_border, float start_offset);
```


### Signal Strength Meter
```c++
draw_signal_strength(int x, int y, int width, int height, int value, int bar_width);

draw_signal_strength(int x, int y, int width, int height, int value);
```

### Thermometer
```c++
draw_thermometer(int x, int y, int width, int height, int value);
```

### Utility Functions (Shared);
```c++
draw_vertical_scale(int x, int y, int height, int spacing, int divisions);

draw_horizontal_scale(int x, int y, int width, int spacing, int divisions);
```
