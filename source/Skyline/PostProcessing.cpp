#include "PostProcessing.h"

PostProcessing::PostProcessing() {
  _game = Game::GetInstance();

  _horizontalBlur = new HorizontalBlur(2);
  _horizontalBlur->initShader();
  _verticalBlur = new VerticalBlur(2);
  _verticalBlur->initShader();
  _horizontalBlur2 = new HorizontalBlur(8);
  _horizontalBlur2->initShader();
  _verticalBlur2 = new VerticalBlur(8);
  _verticalBlur2->initShader();

  _highlightsFilter = new HighlightsFilter(0.5);
  _highlightsFilter->initShader();

  _combineFilter = new CombineFilter();
  _combineFilter->initShader();
  _combineFilter->setBlendMode(CombineFilterBlendMode::SCREEN);
  _combineFilter->setBloomIntensity(1.2f);
}

PostProcessing::~PostProcessing() {
  delete _horizontalBlur;
  delete _verticalBlur;
  delete _horizontalBlur2;
  delete _verticalBlur2;
  delete _highlightsFilter;
  delete _combineFilter;
}

void PostProcessing::applyEffects(Ess2D::FrameBufferObject* _inputFBO) {
  _highlightsFilter->render(_inputFBO);
  _horizontalBlur->render(_highlightsFilter->getResult());
  _verticalBlur->render(_horizontalBlur->getResult());
  _horizontalBlur2->render(_verticalBlur->getResult());
  _verticalBlur2->render(_horizontalBlur2->getResult());
  _combineFilter->render(_inputFBO, _verticalBlur2->getResult());
  _result = _combineFilter->getResult();
}

Ess2D::FrameBufferObject* PostProcessing::getResult() {
  return _result;
}