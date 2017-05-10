#include "b2GLDebugDraw.h"
#include <GL/glew.h>

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
  delete[] vert;

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

  glBindVertexArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
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
  delete [] vert;
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

  glBindVertexArray(0);
  /*i guess it's fine for now since it's just a debug function, 
  but maybe in the future i can make this whole thing more elegant rather than creating and deleting buffers and vertex arrays for every damn polygon.
  */
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void b2GLDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
  const float32 k_segments = 16.0f;
  const int vertexCount = 16;
  const float32 k_increment = 2.0f * b2_pi / k_segments;
  float32 theta = 0.0f;

  GLfloat* vert = new GLfloat[vertexCount * 6];
  for(int i = 0; i < vertexCount; i++) {
    b2Vec2 v = center + radius * b2Vec2(cos(theta), sin(theta));
    vert[i * 6] = v.x;
    vert[i * 6 + 1] = v.y;
    vert[i * 6 + 2] = color.r;
    vert[i * 6 + 3] = color.g;
    vert[i * 6 + 4] = color.b;
    vert[i * 6 + 5] = 0.5f;
    theta += k_increment;
  }

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vert) * vertexCount * 6, vert, GL_DYNAMIC_DRAW);
  delete[] vert;

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

  glBindVertexArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void b2GLDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
  const float32 k_segments = 16.0f;
  const int vertexCount = 16;
  const float32 k_increment = 2.0f * b2_pi / k_segments;
  float32 theta = 0.0f;

  GLfloat* vert = new GLfloat[vertexCount * 6];
  for(int i = 0; i < vertexCount; i++) {
    b2Vec2 v = center + radius * b2Vec2(cos(theta), sin(theta));
    vert[i * 6] = v.x;
    vert[i * 6 + 1] = v.y;
    vert[i * 6 + 2] = color.r;
    vert[i * 6 + 3] = color.g;
    vert[i * 6 + 4] = color.b;
    vert[i * 6 + 5] = 0.5f;
    theta += k_increment;
  }

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vert) * vertexCount * 6, vert, GL_DYNAMIC_DRAW);
  delete [] vert;

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

  glBindVertexArray(0);

  DrawSegment(center, center + radius*axis, color);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void b2GLDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
  int vertexCount = 2;
  GLfloat vert[] = {
    p1.x, p1.y, color.r, color.g, color.b, 0.5f,
    p2.x, p2.y, color.r, color.g, color.b, 0.5f
  };
  
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount * 6, vert, GL_DYNAMIC_DRAW);


  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glDrawArrays(GL_LINES, 0, vertexCount);

  glBindVertexArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void b2GLDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
  int vertexCount = 1;
  GLfloat vert[] = {
    p.x, p.y, color.r, color.g, color.b, 0.5f
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount * 6, vert, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));

  glPointSize(1.0f);
  glDrawArrays(GL_POINTS, 0, vertexCount);

  glBindVertexArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

}

void b2GLDebugDraw::DrawString(int x, int y, const char *string, ...) {
  // TODO:
}

void b2GLDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color) {
  int vertexCount = 4;
  GLfloat vert[] = {
    aabb->lowerBound.x, aabb->lowerBound.y, color.r, color.g, color.b, 0.5f,
    aabb->upperBound.x, aabb->lowerBound.y, color.r, color.g, color.b, 0.5f,
    aabb->upperBound.x, aabb->upperBound.y, color.r, color.g, color.b, 0.5f,
    aabb->lowerBound.x, aabb->upperBound.y, color.r, color.g, color.b, 0.5f,
  };

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
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void b2GLDebugDraw::DrawTransform(const b2Transform& xf) {
  b2Vec2 p1 = xf.p, p2;
  const float32 k_axisScale = 0.4f;

  p2 = p1 + k_axisScale * xf.q.GetXAxis();
  DrawSegment(p1, p2, b2Color(1, 0, 0));

  p2 = p1 + k_axisScale * xf.q.GetYAxis();
  DrawSegment(p1, p2, b2Color(0, 1, 0));
}