#include "calculation_controller.h"
#include <assert.h>

namespace Probability {

CalculationController::ContentView::ContentView(Responder * parentResponder, Law * law) :
  m_lawCurveView(LawCurveView(law)),
  m_imageTableView(ImageTableView(parentResponder, law)),
  m_law(law)
{
  for (int k = 0; k < k_maxNumberOfEditableFields; k++) {
    m_calculationCell[k].setParentResponder(parentResponder);
  }
}

int CalculationController::ContentView::numberOfSubviews() const {
  if ((int)m_law->calculationType() == 1) {
    return 8;
  }
  return 6;
}

View * CalculationController::ContentView::subviewAtIndex(int index) {
  assert(index >= 0 && index < 8);
  if (index == 0) {
    return &m_lawCurveView;
  }
  if (index == 1) {
    return &m_imageTableView;
  }
  if (index == 2) {
    if ((int)m_law->calculationType() == 0) {
      m_text[0].setText("P(X <= ");
    } else {
      m_text[0].setText("P( ");
    }
    m_text[0].setAlignment(0.5f, 0.5f);
    return &m_text[0];
  }
  if (index == 3) {
    m_calculationCell[0].setText("test2");
    return &m_calculationCell[0];
  }
  if (index == 4) {
    if ((int)m_law->calculationType() == 0) {
      m_text[1].setText(") = ");
    }
    if ((int)m_law->calculationType() == 1) {
      m_text[1].setText(" <= X <= ");
    }
    if ((int)m_law->calculationType() == 2) {
      m_text[1].setText(" <= X ) = ");
    }
    m_text[1].setAlignment(0.5f, 0.5f);
    return &m_text[1];
  }
  if (index == 5) {
    m_calculationCell[1].setText("test");
    return &m_calculationCell[1];
  }
  if (index == 6) {
    m_text[2].setText(" )= ");
    m_text[2].setAlignment(0.5f, 0.5f);
    return &m_text[2];
  }
  if (index == 7) {
    m_calculationCell[2].setText("test3");
    return &m_calculationCell[2];
  }
  return nullptr;
}

void CalculationController::ContentView::layoutSubviews() {
  markRectAsDirty(bounds());
  KDCoordinate xCoordinate = 0;
  m_lawCurveView.setFrame(KDRect(0,  ImageTableView::k_imageHeight, bounds().width(), bounds().height() - ImageTableView::k_imageHeight));
  m_imageTableView.setFrame(KDRect(xCoordinate, 0, ImageTableView::k_imageWidth, 3*ImageTableView::k_imageHeight));
  xCoordinate += ImageTableView::k_imageWidth + k_textMargin;
  KDCoordinate numberOfCharacters = 7;
  if ((int)m_law->calculationType() > 0) {
    numberOfCharacters = 3;
  }
  m_text[0].setFrame(KDRect(xCoordinate, 0, numberOfCharacters*k_charWidth, ImageTableView::k_imageHeight));
  xCoordinate += numberOfCharacters*k_charWidth + k_textMargin;
  m_calculationCell[0].setFrame(KDRect(xCoordinate, 0, k_textFieldWidth, ImageTableView::k_imageHeight));
  xCoordinate += k_textFieldWidth + k_textMargin;
  if ((int)m_law->calculationType() == 0) {
    numberOfCharacters = 4;
  }
  if ((int)m_law->calculationType() == 1) {
    numberOfCharacters = 9;
  }
  if ((int)m_law->calculationType() == 2) {
    numberOfCharacters = 10;
  }
  m_text[1].setFrame(KDRect(xCoordinate, 0, numberOfCharacters*k_charWidth, ImageTableView::k_imageHeight));
  xCoordinate += numberOfCharacters*k_charWidth + k_textMargin;
  m_calculationCell[1].setFrame(KDRect(xCoordinate, 0, k_textFieldWidth, ImageTableView::k_imageHeight));
  xCoordinate += k_textFieldWidth + k_textMargin;
  numberOfCharacters = 4;
  m_text[2].setFrame(KDRect(xCoordinate, 0, numberOfCharacters*k_charWidth, ImageTableView::k_imageHeight));
  xCoordinate += numberOfCharacters*k_charWidth + k_textMargin;
  m_calculationCell[2].setFrame(KDRect(xCoordinate, 0, k_textFieldWidth, ImageTableView::k_imageHeight));
}


void CalculationController::ContentView::drawRect(KDContext * ctx, KDRect rect) const {
  ctx->fillRect(bounds(), KDColorWhite);
}

LawCurveView * CalculationController::ContentView::lawCurveView() {
  return &m_lawCurveView;
}

ImageTableView * CalculationController::ContentView::imageTableView() {
  return &m_imageTableView;
}

EditableTextCell * CalculationController::ContentView::calculationCellAtIndex(int index) {
  return &m_calculationCell[index];
}

CalculationController::CalculationController(Responder * parentResponder, Law * law) :
  ViewController(parentResponder),
  m_contentView(ContentView(this, law)),
  m_highlightedSubviewIndex(1)
{
}

View * CalculationController::view() {
  return &m_contentView;
}

const char * CalculationController::title() const {
  return "Calculer les probabilites";
}

bool CalculationController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::OK) {
    if (m_highlightedSubviewIndex == 0) {
      m_contentView.imageTableView()->select(true);
      app()->setFirstResponder(m_contentView.imageTableView());
      return true;
    }
  }
  if ((event == Ion::Events::Left && m_highlightedSubviewIndex > 0) || (event == Ion::Events::Right && m_highlightedSubviewIndex < ContentView::k_maxNumberOfEditableFields - 1)) {
    if (m_highlightedSubviewIndex == 0) {
      app()->setFirstResponder(this);
      m_contentView.imageTableView()->select(false);
      m_contentView.imageTableView()->setHighlight(false);
    } else {
      EditableTextCell * calculCell = m_contentView.calculationCellAtIndex(m_highlightedSubviewIndex-1);
      calculCell->setHighlighted(false);
    }
    m_highlightedSubviewIndex = event == Ion::Events::Left ? m_highlightedSubviewIndex - 1 : m_highlightedSubviewIndex + 1;
    if (m_highlightedSubviewIndex > 0) {
      EditableTextCell * newCalculCell = m_contentView.calculationCellAtIndex(m_highlightedSubviewIndex-1);
      newCalculCell->setHighlighted(true);
    } else {
      m_contentView.imageTableView()->setHighlight(true);
    }
    return true;
  }
  return false;
}

void CalculationController::didBecomeFirstResponder() {
  m_contentView.layoutSubviews();
  if (m_highlightedSubviewIndex > 0) {
    EditableTextCell * calculCell = m_contentView.calculationCellAtIndex(m_highlightedSubviewIndex-1);
    calculCell->setHighlighted(true);
  } else {
    m_contentView.imageTableView()->setHighlight(true);
  }
  m_contentView.lawCurveView()->reload();
}

}
