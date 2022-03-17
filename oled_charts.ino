/// SHARED FUNCTIONS

void draw_vertical_scale(int x, int y, int height, int spacing, int divisions) {
  int bottom = y + height - 1;

  for (int i = 0; i <= height/spacing; i++) {
    int tick_y = bottom - i * spacing;
    display.drawPixel(x, tick_y, SSD1306_INVERSE);
    if (i % divisions == 0) {
      display.drawPixel(x + 1, tick_y, SSD1306_INVERSE);
    }
  }
}

void draw_horizontal_scale(int x, int y, int width, int spacing, int divisions) {
  for (int i = 0; i < width/spacing; i ++) {
    int tick_x = x + i * spacing;
    display.drawPixel(tick_x, y, SSD1306_INVERSE);
    if (i % divisions == 0) {
      display.drawPixel(tick_x, y - 1, SSD1306_INVERSE);
    }
  }
}


/// GRAPHS


/*
 * Line Graph
 */
void draw_line_graph(int x, int y, int width, int height, int data[], int data_count, bool line, bool fill) {
  int bottom = y + height-1;

  for (int i = 0; i < min(data_count, width); i++) {
    int bar_y = bottom - constrain(map(data[i], 0, 100, 0, height-1), 0, height-1);

    if (fill) {
      display.drawLine(x + i, bar_y, x + i, bottom, SSD1306_WHITE);
    } else if (line && i > 0) {
      int last_bar_y = bottom - constrain(map(data[i-1], 0, 100, 0, height-1), 0, height-1);
      display.drawLine(x + (i-1), last_bar_y, x + i, bar_y, SSD1306_WHITE);
    } else {
      display.drawPixel(x + i, bar_y, SSD1306_WHITE);
    }
  }

  draw_vertical_scale(x, y, height, 3, 5);
  draw_horizontal_scale(x, bottom, width, 3, 5);
}

void draw_line_graph(int x, int y, int width, int height, int data[], int data_count) {
 draw_line_graph(x, y, width, height, data, data_count, true, false);
}


/*
 * Area Graph
 */
void draw_area_graph(int x, int y, int width, int height, int data[], int data_count) {
 draw_line_graph(x, y, width, height, data, data_count, false, true);
}


/*
 * Dot Graph
 */
void draw_dot_graph(int x, int y, int width, int height, int data[], int data_count) {
draw_line_graph(x, y, width, height, data, data_count, false, false);
}


/*
 * Bar Graph
 */
void draw_bar_graph(int x, int y, int width, int height, int data[], int data_count, int bar_width, int bar_padding) {
  int bottom = y + height - 1;
  int bar_start_x = x + 2;
  int bar_count = min(data_count, width / (bar_width + bar_padding));

  for (int i = 0; i < bar_count; i++) {
    int bar_height = constrain(map(data[i], 0, 100, 0, height - 1), 0, height - 1);
    int offset_x = bar_start_x + (i *  (bar_width+bar_padding));
    display.fillRect(offset_x, bottom - bar_height, bar_width, bar_height, SSD1306_WHITE);
  }

  draw_vertical_scale(x, y, height, 3, 5);
  display.drawFastHLine(x, bottom, width, SSD1306_WHITE);
}

void draw_bar_graph(int x, int y, int width, int height, int data[], int data_count) {
  draw_bar_graph(x, y, width, height, data, data_count, 3, 2);
}

void draw_autoscale_bar_graph(int x, int y, int width, int height, int data[], int data_count) {
  int bar_padding = 3;

  if (data_count > 4) bar_padding = 2;
  if (data_count > 8) bar_padding = 1;

  int bar_width = max(1, (width - (data_count * bar_padding)) / data_count);

  draw_bar_graph(x, y, width, height, data, data_count, bar_width, bar_padding);
}


/*
 * Linear Gauge
 */
void draw_linear_gauge(int x, int y, int width, int height, int value) {
  int needle_x = map(value, 0, 100, x, x + width);
  int needle_height = 3 * height / 5;
  int needle_y = y + (height - needle_height);

  draw_horizontal_scale(x, needle_y, width, 2, 10);

  // Draw frame
  display.drawLine(x, needle_y+2, x+width-1, needle_y+2, SSD1306_WHITE);
  display.drawLine(x, y, x, needle_y+4, SSD1306_WHITE);
  display.drawLine(x+width-1, y, x+width-1, needle_y+4, SSD1306_WHITE);

  // Draw needle
  display.fillTriangle(needle_x, needle_y, needle_x-4, y+height, needle_x+4, y+height, SSD1306_WHITE);
  display.drawLine(needle_x, needle_y, needle_x, y + height, SSD1306_BLACK);
}


/*
 * Needle Meter
 */
void draw_needle_meter(int x, int y, int width, int value) {
  int radius = width;
  int circle_x = x+width/2;
  int circle_y = y+radius;
  int needle_length = radius-4;
  int needle_taper = radius-15;

  for (int i = 0; i <= 20; i++) {
    float mapped = ((constrain(i*5, 0, 100)-50)*0.01) - PI/2;
    float xoff = cos(mapped);
    float yoff = sin(mapped);
    int length = 4;
    if (i % 5 == 0) length = 10;
    display.drawLine(circle_x + xoff * radius,
                     circle_y + yoff * radius,
                     circle_x + xoff * (radius - length),
                     circle_y + yoff * (radius - length),
                     SSD1306_WHITE);
  }

  float mapped = ((constrain(value, 0, 100)-50)*0.01) - PI/2;
  float xoff = cos(mapped);
  float yoff = sin(mapped);
  display.drawLine(circle_x, circle_y, circle_x + xoff * needle_length, circle_y + yoff * needle_length, SSD1306_WHITE);
  display.drawLine(circle_x+1, circle_y, circle_x + 1 + xoff * needle_taper, circle_y + yoff * needle_taper, SSD1306_WHITE);
  display.drawLine(circle_x-1, circle_y, circle_x - 1 + xoff * needle_taper, circle_y + yoff * needle_taper, SSD1306_WHITE);
}


/*
 * Signal Strength
 */
void draw_signal_strength(int x, int y, int width, int height, int value, int bar_width) {
  int highest = map(value, 0, 100, 0, width/(1+bar_width));

  for (int i = 0; i < highest; i++) {
    int bar_height = map(i*(1+bar_width), 0, width, 0, height);
    display.fillRect(x + i*(1+bar_width), y + height - bar_height,
                     bar_width, bar_height,
                     SSD1306_WHITE);
  }
}

void draw_signal_strength(int x, int y, int width, int height, int value) {
  draw_signal_strength(x, y, width, height, value, 1);
}


/*
 * Thermometer
 */
void draw_thermometer(int x, int y, int width, int height, int value) {
  int thickness = 2;
  if (width > 20 && height > 20) thickness = 3;
  if (width < 8) thickness = 1;
  int corner_radius = thickness * 3;

  int bulb_radius = max(1, (width/2) - thickness * 2);
  int bulb_x = x + bulb_radius + thickness * 2;
  int bulb_y = y + height - bulb_radius - thickness * 2 - 1;

  int bar_width = max(1, (width/2) - thickness * 4);
  int bar_x = bulb_x - bar_width/2;
  int bar_y = y + thickness * 2;
  int bar_bottom_y = bulb_y - bulb_radius - thickness * 2;
  int bar_max_height = abs(bar_y - bar_bottom_y);

  int bar_height = map(value, 0, 100, 0, bar_max_height) + corner_radius;
  int scale_x = bulb_x + bar_width/2 + thickness*3;
  int scale_y = y + thickness * 3 - 1;

  // Outline
  draw_vertical_scale(scale_x, scale_y, bar_max_height, 3, 5);
  display.fillRoundRect(bar_x-thickness*2, y, bar_width+thickness*4,
                        height-bulb_radius, corner_radius,
                        SSD1306_WHITE);
  display.fillCircle(bulb_x, bulb_y, bulb_radius+thickness*2, SSD1306_WHITE);
  display.fillRoundRect(bar_x-thickness*1, y+thickness, bar_width+thickness*2,
                        height-bulb_radius-thickness*2, corner_radius,
                        SSD1306_BLACK);
  display.fillCircle(bulb_x, bulb_y, bulb_radius+thickness*1, SSD1306_BLACK);

  // Inner
  display.fillRect(bar_x, bar_bottom_y+1, bar_width, bulb_radius, SSD1306_WHITE);
  display.fillCircle(bulb_x, bulb_y, bulb_radius, SSD1306_WHITE);
  display.fillRoundRect(bar_x, bar_bottom_y - bar_height + corner_radius,
                        bar_width, bar_height, corner_radius, SSD1306_WHITE);
}


/*
 * Segmented Gauge
 */
void draw_segmented_gauge(int x, int y, int width, int height, int value, int segments) {
  segments = constrain(segments, 1, width/5);
  int margin = 2;
  int segment_width = (width / segments) - margin;
  int highlight_to_x = map(value, 0, 100, 0, width);

  for (int i = 0; i < segments; i++) {
    int offset = (segment_width + margin) * i;
    if (offset < highlight_to_x) {
      display.fillRect(x + offset, y, segment_width, height, SSD1306_WHITE);
    } else {
      display.drawRect(x + offset, y, segment_width, height, SSD1306_WHITE);
    }
  }
}


/*
 * Dot Gauge
 */
void draw_dot_gauge(int x, int y, int width, int height, int value) {
  int margin = 2;
  int segment_radius = (height/2);
  int segments = width / (segment_radius*2+margin);
  int highlight_to_x = map(value, 0, 100, 0, width);

  for (int i = 0; i < segments; i++) {
    int offset = ((segment_radius*2 + margin) * i) + segment_radius;
    if (offset < highlight_to_x) {
      display.fillCircle(x + offset, y+segment_radius, segment_radius, SSD1306_WHITE);
    } else {
      display.drawCircle(x + offset, y+segment_radius, segment_radius, SSD1306_WHITE);
    }
  }
}


/*
 * Radial Gauge
 */
void draw_radial_gauge(int x, int y, int radius, int value, int padding, bool outer_border, bool inner_border, bool draw_line, float start_offset) {
  int segments = 32;
  int inner_radius = radius * 0.6;
  int meter_radius = radius - padding;
  float segment_arc = (2 * PI) / (segments);
  float half_arc = segment_arc / 2;
  int fill_up_to = constrain(map(value, 0, 100, 0, segments), 0, 100);

  for (int i = 0; i < fill_up_to; i++) {
    float segment_theta = i * (segment_arc) + start_offset;
    display.fillTriangle(x,
                         y,
                         x+(cos(segment_theta - half_arc) * meter_radius),
                         y+(sin(segment_theta - half_arc) * meter_radius),
                         x+(cos(segment_theta + half_arc) * meter_radius),
                         y+(sin(segment_theta + half_arc) * meter_radius),
                         SSD1306_WHITE);
  }

  display.fillCircle(x, y, inner_radius, SSD1306_BLACK);

  if (outer_border) display.drawCircle(x, y, radius, SSD1306_WHITE);
  if (inner_border) display.drawCircle(x, y, inner_radius - padding, SSD1306_WHITE);
  if (draw_line) {
    float x1 = cos((fill_up_to) * segment_arc + start_offset);
    float y1 = sin((fill_up_to) * segment_arc + start_offset);
    display.drawLine(x+(x1*radius), y+(y1*radius), x+(x1*radius*0.4), y+(y1*radius*0.4), SSD1306_WHITE);
  }
}

void draw_radial_gauge(int x, int y, int radius, int value) {
  draw_radial_gauge(x, y, radius, value, 2, true, true, true, -PI/2);
}

void draw_radial_gauge(int x, int y, int radius, int value, int padding) {
  draw_radial_gauge(x, y, radius, value, padding, true, true, true, -PI/2);
}


/*
 * Radial Dot Gauge
 */
void draw_radial_dot_gauge(int x, int y, int radius, int dot_radius, int value, int segments, int empty_dot_radius, float start_offset) {
  float segment_arc = (2 * PI) / (segments);
  int fill_up_to = constrain(map(value, 0, 100, 0, segments), 0, 100);

  for (int i = 0; i < segments; i++) {
    float segment_theta = i * (segment_arc) + start_offset;
    if (i <= fill_up_to) {
      display.fillCircle(x+(cos(segment_theta) * radius),
                         y+(sin(segment_theta) * radius),
                         dot_radius,
                         SSD1306_WHITE);
    } else {
      display.drawCircle(x+(cos(segment_theta) * radius),
                         y+(sin(segment_theta) * radius),
                         empty_dot_radius,
                         SSD1306_WHITE);
    }
  }
}

void draw_radial_dot_gauge(int x, int y, int radius, int dot_radius, int value) {
  draw_radial_dot_gauge(x, y, radius, dot_radius, value, 8, 1, -PI/2);
}


/*
 * Radial Segment Gauge
 */
void draw_radial_segment_gauge(int x, int y, int radius, int segments, int value, int padding, bool outer_border, bool inner_border,  float start_offset) {
  float segment_arc = (2 * PI) / (segments);
  float half_arc = segment_arc / (sqrt(radius) - 1);
  int fill_up_to = constrain(map(value, 0, 100, 0, segments), 0, 100);

  for (int i = 0; i < fill_up_to; i++) {
    float segment_theta = i * (segment_arc) + start_offset;
    display.fillTriangle(x+(cos(segment_theta) * (radius / 3 - padding)),
                         y+(sin(segment_theta) * (radius / 3 - padding)),
                         x+(cos(segment_theta - half_arc) * (radius - padding)),
                         y+(sin(segment_theta - half_arc) * (radius - padding)),
                         x+(cos(segment_theta + half_arc) * (radius - padding)),
                         y+(sin(segment_theta + half_arc) * (radius - padding)),
                         SSD1306_WHITE);
  }

  display.fillCircle(x, y, radius * 0.5, SSD1306_BLACK);
  if (inner_border) display.drawCircle(x, y, (radius* 0.5) - padding, SSD1306_WHITE);
  if (outer_border) display.drawCircle(x, y, radius, SSD1306_WHITE);
}

void draw_radial_segment_gauge(int x, int y, int radius, int segments, int value) {
  draw_radial_segment_gauge(x, y, radius, segments, value, 2, true, true, -PI/2);
}


/*
 * Radial Line Gauge
 */
void draw_radial_line_gauge(int x, int y, int radius, int lines, int value, bool outer_border, bool inner_border, float start_offset) {
  float segment_arc = (2 * PI) / lines;
  float half_arc = segment_arc / 4;
  int fill_up_to = constrain(map(value, 0, 100, 0, lines), 0, 100);

  for (int i = 0; i < fill_up_to; i++) {
    float segment_theta = i * (segment_arc) + start_offset;
    int x1 = cos(segment_theta) * radius;
    int y1 = sin(segment_theta) * radius;
    if (i <= fill_up_to) {
      display.drawLine(x, y, x+x1, y+y1, SSD1306_WHITE);
    }
  }

  display.fillCircle(x, y, radius * 0.4, SSD1306_BLACK);
  if (outer_border) display.drawCircle(x, y, radius, SSD1306_WHITE);
  if (inner_border) display.drawCircle(x, y, (radius * 0.4), SSD1306_WHITE);
}

void draw_radial_line_gauge(int x, int y, int radius, int lines, int value) {
  draw_radial_line_gauge(x, y, radius, lines, value, true, true, -PI/2);
}
