#include "b2GLDebugDraw.h"
#include <glew.h>

void b2GLDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
  GLfloat* vert = new GLfloat[vertexCount * 6];

  for(int i = 0; i < vertexCount; i++) {
    b2Vec2 v = vertices[i];
    vert[i * 6] = v.x;
    vert[i * 6 + 1] = v.y;
    vert[i * 6 + 2] = color.r;
    vert[i * 6 + 3] = color.g;
    vert[i * 6 + 4] = color.b;
    vert[i * 6 + 5] = 0.5f;
  }

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

  glBindVertexArray(0);
}

void b2GLDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
  GLfloat* vert = new GLfloat[vertexCount * 6];

  for(int i = 0; i < vertexCount; i++) {
    b2Vec2 v = vertices[i];
    vert[i * 6] = v.x;
    vert[i * 6 + 1] = v.y;
    vert[i * 6 + 2] = color.r;
    vert[i * 6 + 3] = color.g;
    vert[i * 6 + 4] = color.b;
    vert[i * 6 + 5] = 0.5f;
  }

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vert) * vertexCount * 6, vert, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

  glBindVertexArray(0);
  /*
  glColor4f(color.r, color.g, color.b, 0.5f);
  glBegin(GL_TRIANGLE_FAN);
  for(int i = 0; i < vertexCount; i++) {
    b2Vec2 v = vertices[i];
    glVertex2f(v.x, v.y);
  }
  glEnd();*/
}

void b2GLDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {

  const float32 k_segments = 16.0f;
  const int vertexCount = 16;
  const float32 k_increment = 2.0f * b2_pi / k_segments;
  float32 theta = 0.0f;

  glColor4f(color.r, color.g, color.b, 1);
  glBegin(GL_LINE_LOOP);
  GLfloat glVertices[vertexCount * 2];
  for(int32 i = 0; i < k_segments; ++i) {
    b2Vec2 v = center + radius * b2Vec2(cos(theta), sin(theta));
    glVertex2f(v.x, v.y);
    theta += k_increment;
  }
  glEnd();
}

void b2GLDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
  const float32 k_segments = 16.0f;
  const int vertexCount = 16;
  const float32 k_increment = 2.0f * b2_pi / k_segments;
  float32 theta = 0.0f;

  glColor4f(color.r, color.g, color.b, 0.5f);
  glBegin(GL_TRIANGLE_FAN);
  GLfloat glVertices[vertexCount * 2];
  for(int32 i = 0; i < k_segments; ++i) {
    b2Vec2 v = center + radius * b2Vec2(cos(theta), sin(theta));
    glVertex2f(v.x, v.y);
    theta += k_increment;
  }
  glEnd();

  DrawSegment(center, center + radius*axis, color);
}

void b2GLDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
  glColor4f(color.r, color.g, color.b, 1);
  glBegin(GL_LINES);
  glVertex2f(p1.x, p1.y);
  glVertex2f(p2.x, p2.y);
  glEnd();
}

void b2GLDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
  glColor4f(color.r, color.g, color.b, 1);
  glPointSize(1.0f);
  glBegin(GL_POINTS);
  glVertex2f(p.x, p.y);
  glEnd();
}

void b2GLDebugDraw::DrawString(int x, int y, const char *string, ...) {
  // TODO:
}

void b2GLDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c) {
  glColor4f(c.r, c.g, c.b, 1);
  glBegin(GL_LINE_LOOP);
  glVertex2f(aabb->lowerBound.x, aabb->lowerBound.y);
  glVertex2f(aabb->upperBound.x, aabb->lowerBound.y);
  glVertex2f(aabb->upperBound.x, aabb->upperBound.y);
  glVertex2f(aabb->lowerBound.x, aabb->upperBound.y);
  glEnd();
}

void b2GLDebugDraw::DrawTransform(const b2Transform& xf) {
  b2Vec2 p1 = xf.p, p2;
  const float32 k_axisScale = 0.4f;

  p2 = p1 + k_axisScale * xf.q.GetXAxis();
  DrawSegment(p1, p2, b2Color(1, 0, 0));

  p2 = p1 + k_axisScale * xf.q.GetYAxis();
  DrawSegment(p1, p2, b2Color(0, 1, 0));
}