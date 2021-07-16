#ifndef _MATERIALIZE_LAYOUT_ELEMENT_H_
#define _MATERIALIZE_LAYOUT_ELEMENT_H_

#include <Arduino.h>
#include <memory>
#include <variant>
#include <vector>

#include "../DynamicComponentRegistrationService/DynamicComponentRegistrationService.h"
#include "../Container/Container.h"

enum class LayoutTypes
{
  NONE,
  ROW,
  COLUMN
};

template <typename T>
class Layout : public Container<T>
{
  template <typename U>
  friend class Tab;
  friend class Page;

private:
  std::vector<std::shared_ptr<Layout<T>>> layout;
  LayoutTypes layoutType = LayoutTypes::NONE;
  using Container<T>::getDynamicComponentRegistrationServerPtr;
  bool vAlign = false;

protected:
  virtual String getHTML();
  virtual bool emit(size_t id, String value);

public:
  Layout(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, LayoutTypes lt = LayoutTypes::NONE);
  void setVerticalAlign(bool state);

  std::shared_ptr<Layout<T>> createRow();
  std::shared_ptr<Layout<T>> createColumn();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Layout<T>::Layout(std::shared_ptr<DynamicComponentRegistrationService<T>> registrationService, LayoutTypes lt) : Container<T>(registrationService)
{
  this->layoutType = lt;
}

template <typename T>
String Layout<T>::getHTML()
{
  String componentTemplate, nestedLayout, className;

  for (auto l : this->layout)
  {
    if (l)
    {
      nestedLayout += l->getHTML();
    }
  }

  switch (this->layoutType)
  {
  case LayoutTypes::NONE:
  default:
    break;
  case LayoutTypes::ROW:
    className = "row";
    break;
  case LayoutTypes::COLUMN:
    className = "col";
    break;
  }

  componentTemplate = F("<div class=\"");
  componentTemplate += className;
  componentTemplate += " ";
  componentTemplate += this->getWidthClass();
  if (this->vAlign)
  {
    componentTemplate += F(" valign-wrapper");
  }
  componentTemplate += F("\">");
  componentTemplate += this->compileComponents();
  componentTemplate += nestedLayout;
  componentTemplate += F("</div>");

  return componentTemplate;
}

template <typename T>
std::shared_ptr<Layout<T>> Layout<T>::createRow()
{
  auto r = std::make_shared<Layout<T>>(this->getDynamicComponentRegistrationServerPtr(), LayoutTypes::ROW);
  this->layout.push_back(r);

  return r;
}

template <typename T>
std::shared_ptr<Layout<T>> Layout<T>::createColumn()
{
  auto c = std::make_shared<Layout<T>>(this->getDynamicComponentRegistrationServerPtr(), LayoutTypes::COLUMN);
  this->layout.push_back(c);

  return c;
}

template <typename T>
bool Layout<T>::emit(size_t id, String value)
{
  bool found = false;
  if (this->emitOnContainer(id, value))
    found = true;
  else
    for (auto l : this->layout)
    {
      if (l)
        found = l->emit(id, value);
      if (found)
        break;
    }

  return found;
}

template <typename T>
void Layout<T>::setVerticalAlign(bool state)
{
  this->vAlign = state;
}

#endif //_MATERIALIZE_LAYOUT_H_