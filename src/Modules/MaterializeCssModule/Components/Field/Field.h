#pragma once

#include <Arduino.h>

#include "../BaseClass/MaterializeCssBaseClass.h"

#include "../Input/Input.h"

template <typename T>
class Field : public MaterializeCssBaseClass<T> {
  private:
  using MaterializeCssBaseClass<T>::appendChild;
  using MaterializeCssBaseClass<T>::removeAllChildren;
  using MaterializeCssBaseClass<T>::removeChild;

  String name;

  public:
  Input<T> inputElement;

  /**
   * @brief Constructs a new Field object
   *
   * @param name the name of the field
   * @param defaultValue default value of the field
   */
  Field(T* argCollection);

  virtual ~Field() = default;

  String getHTML();

  /**
   * @brief Gets the name of the field
   *
   * @return String
   */
  String getName();

  /**
   * @brief Sets the name of the field
   *
   * @param name the new name of the field
   *
   * @return void
   */
  void setName(String name);

  /**
   * @brief Gets the default value of the field
   *
   * @return String
   */
  String getDefaultValue();

  /**
   * @brief Sets the default value of the field
   *
   * @param defaultValue default value of the field
   *
   * @return void
   */
  void setDefaultValue(String defaultValue);

  /**
   * @brief Gets the text that is currently written in the field
   *
   * @return String
   */
  String getCurrentValue();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Field<T>::Field(T* argCollection)
    : MaterializeCssBaseClass<T>(argCollection)
    , inputElement(argCollection)

{
  this->classList.add(F("input-field"));
  this->classList.add(F("col"));

  this->appendChild(&this->inputElement);
}

template <typename T>
String Field<T>::getName()
{
  return this->name;
}

template <typename T>
void Field<T>::setName(String name)
{
  name.trim();
  this->name = name;
}

template <typename T>
String Field<T>::getDefaultValue()
{
  return this->inputElement.getAttribute(F("value"));
}

template <typename T>
void Field<T>::setDefaultValue(String defaultValue)
{
  defaultValue.trim();
  this->inputElement.setAttribute(F("value"), defaultValue, false);
}

template <typename T>
String Field<T>::getCurrentValue()
{
  return this->inputElement.getCurrentValue();
}

template <typename T>
String Field<T>::getHTML()
{
  String id = (String)this->getId();

  String elemTemplate = F("<div ");
  elemTemplate += this->getAttributes();
  elemTemplate += F(">");
  elemTemplate += this->inputElement.getHTML();
  elemTemplate += F("<label for=\"");
  elemTemplate += String(this->inputElement.getId());
  elemTemplate += F("\">");
  elemTemplate += this->name;
  elemTemplate += F("</label></div>");

  return elemTemplate;
}
