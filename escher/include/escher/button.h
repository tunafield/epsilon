#ifndef ESCHER_BUTTON_H
#define ESCHER_BUTTON_H

#include <escher/view.h>
#include <escher/responder.h>
#include <escher/text_view.h>
#include <escher/invocation.h>

class Button : public View, public Responder {
public:
  Button(Responder * parentResponder);
  void drawRect(KDContext * ctx, KDRect rect) const override;
  bool handleEvent(Ion::Events::Event event) override;
  void setText(const char * textBody);
  void setInvocation(Invocation invocation);
  void setBackgroundColor(KDColor backgroundColor);
  KDSize textSize();
private:
  int numberOfSubviews() const override;
  View * subviewAtIndex(int index) override;
  void layoutSubviews() override;
  TextView m_textView;
  Invocation m_invocation;
  KDColor m_backgroundColor;
};

#endif