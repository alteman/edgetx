/*
 * Copyright (C) EdgeTX
 *
 * Based on code named
 *   opentx - https://github.com/opentx/opentx
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "input_edit_adv.h"
#include "fm_matrix.h"

#include "opentx.h"

#define SET_DIRTY() storageDirty(EE_MODEL)

static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(2),
                                     LV_GRID_TEMPLATE_LAST};
static const lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

InputEditAdvanced::InputEditAdvanced(uint8_t input_n, uint8_t index) :
    Page(ICON_MODEL_INPUTS)
{
  std::string title2(getSourceString(MIXSRC_FIRST_INPUT + input_n));
  header.setTitle(STR_MENUINPUTS);
  header.setTitle2(title2);

  rect_t r = rect_t{0, 0, body.width(), body.height()};  
  auto form = new FormWindow(&body, r);
    
  FlexGridLayout grid(col_dsc, row_dsc, 2);
  form->setFlexLayout();

  ExpoData* input = expoAddress(index);

  // Side
  auto line = form->newLine(&grid);
  new StaticText(line, rect_t{}, STR_SIDE, 0, COLOR_THEME_PRIMARY1);
  new Choice(
      line, rect_t{}, STR_VCURVEFUNC, 1, 3,
      [=]() -> int16_t { return 4 - input->mode; },
      [=](int16_t newValue) {
        input->mode = 4 - newValue;
        SET_DIRTY();
      });

  // Trim
  line = form->newLine(&grid);
  new StaticText(line, rect_t{}, STR_TRIM, 0, COLOR_THEME_PRIMARY1);
  auto c = new Choice(line, rect_t{}, STR_VMIXTRIMS, -TRIM_OFF,
                          -TRIM_LAST, GET_VALUE(-input->trimSource),
                          SET_VALUE(input->trimSource, -newValue));
  c->setAvailableHandler([=](int value) {
    return value != TRIM_ON || input->srcRaw <= MIXSRC_Ail;
  });

  // Flight modes
  if (modelFMEnabled()) {
    line = form->newLine(&grid);
    new StaticText(line, rect_t{}, STR_FLMODE, 0, COLOR_THEME_PRIMARY1);
    new FMMatrix<ExpoData>(line, rect_t{}, input);
  }
}
