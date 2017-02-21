#pragma once
#include "PostProcessing/HorizontalBlur.h"
#include "PostProcessing/VerticalBlur.h"
#include "PostProcessing/HighlightsFilter.h"
#include "PostProcessing/CombineFilter.h"
#include "Game.h"

class HorizontalBlur;
class VerticalBlur;
class HighlightsFilter;
class CombineFilter;

class PostProcessing {
  public:
    PostProcessing();
    ~PostProcessing();

    void applyEffects(Ess2D::FrameBufferObject* _inputFBO);
    Ess2D::FrameBufferObject* getResult();

  private:
    Game* _game;
    HorizontalBlur* _horizontalBlur;
    VerticalBlur* _verticalBlur;
    HorizontalBlur* _horizontalBlur2;
    VerticalBlur* _verticalBlur2;
    HighlightsFilter* _highlightsFilter;
    CombineFilter* _combineFilter;
    Ess2D::FrameBufferObject* _result;
};

