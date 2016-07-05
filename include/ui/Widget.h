//
// Widget.h
// Vorb Engine
//
// Created by Benjamin Arnold on 27 Apr 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Widget.h
* @brief 
* Base class for all Widgets.
*
*/

#pragma once

#ifndef Vorb_Widget_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Widget_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <vector>
#include "../Events.hpp"
#include "../VorbPreDecl.inl"
#include "IWidgetContainer.h"
#include "Form.h"
#include "../math/TweeningMath.hpp"

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        //! Enum of widget alignments.
        enum class WidgetAlign {
            LEFT,
            TOP_LEFT,
            TOP,
            TOP_RIGHT,
            RIGHT,
            BOTTOM_RIGHT,
            BOTTOM,
            BOTTOM_LEFT,
            CENTER
        };

        //! Enum of position types.
        enum class PositionType {
            STATIC,
            ABSOLUTE,
            FIXED,
            RELATIVE
        };

        //! Enum of docking styles.
        enum class DockingStyle {
            NONE,
            LEFT,
            TOP,
            RIGHT,
            BOTTOM,
            FILL
        };

        //! Struct of docking options.
        struct DockingOptions {
            DockingStyle style;
            Length size;
        };
        
        //! Structs of target length and time frame to completion.
        struct Transition {
            Length rawInitialLength, rawTargetLength;
            f32 initialLength, targetLength; // TODO(Matthew): Ensure these details are updated at appropriate changes to parent.
            ui16 currentTime, finalTime;
            f32(*tweeningFunc)(f32, f32, ui16, ui16);
        };
        struct Transition2 {
            Length2 rawInitialLength, rawTargetLength;
            f32v2 initialLength, targetLength; // TODO(Matthew): Ensure these details are updated at appropriate changes to parent.
            ui16 currentTime, finalTime;
            f32(*tweeningFunc)(f32, f32, ui16, ui16);
        };

        // Forward Declarations
        class UIRenderer;

        class Widget : public IWidgetContainer {
            friend class WidgetScriptFuncs;
            friend class IWidgetContainer;
        public:
            /*! @brief Default constructor. */
            Widget();
            /*! @brief Constructor that sets name, position, and dimensions.
             *
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
            *
            * The widget will be made a child of parent.
            *
            * @param parent: Parent container object.
            * @param name: Name of the control.
            * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
            */
            Widget(Form* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Constructor that sets parent control, name, position, and dimensions.
             *
             * The widget will be made a child of parent.
             *
             * @param parent: Parent container object.
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(Widget* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~Widget();
            /*! @brief Releases all resources used by the Widget.
            *
            * Gets called in the destructor.
            */
            virtual void dispose() override;

            /*! @brief Adds a widget to the Form and initializes it for rendering.
            *
            * @param widget: The Widget to add.
            * @return true on success.
            */
            virtual bool addWidget(Widget* widget);

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to.
            */
            virtual void addDrawables(UIRenderer* renderer);

            /*! @brief Removes all drawables from the UIRenderer */
            virtual void removeDrawables();

            /*! @brief Recalculates order of drawables based on Z-index. */
            virtual void updateDrawableOrderState();


            /*! @brief Updates the widget. Can be used for animation.
            *
            * @param dt: The TimeStep
            */
            virtual void update(f32 dt = 1.0f); // TODO(Matthew): Implement animation using tweening funcs.

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/ 
            virtual const DockingOptions& getDockingOptions() const { return m_dockingOptions; }
            virtual f32 getProcessedDockingSize() const { return m_processedDockingSize; }
            virtual const AnchorStyle& getAnchor() const { return m_anchor; }
            virtual const PositionType& getPositionType() const { return m_positionType; }
            virtual const volatile bool& needsDrawableReload() const { return m_needsDrawableReload; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            virtual const UIRenderer* getRenderer() const { return m_renderer; }
            virtual const Length2& getRawPosition() const { return m_rawPosition; }
            virtual const Transition2& getTargetRawPosition() const { return m_targetRawPosition; }
            virtual const f32v2& getRelativePosition() const { return m_relativePosition; }
            virtual const Length2& getRawDimensions() const { return m_rawDimensions; }
            virtual const Transition2& getTargetRawDimensions() const { return m_targetRawDimensions; }
            virtual const Length2& getRawMinSize() const { return m_rawMinSize; }
            virtual const Transition2& getTargetRawMinSize() const { return m_targetRawMinSize; }
            virtual const f32v2& getMinSize() const { return m_minSize; }
            virtual const Length2& getRawMaxSize() const { return m_rawMaxSize; }
            virtual const Transition2& getTargetRawMaxSize() const { return m_targetRawMaxSize; }
            virtual const f32v2& getMaxSize() const { return m_maxSize; }
            virtual const WidgetAlign& getWidgetAlign() const { return m_align; }
            virtual ui16 getZIndex() const { return m_zIndex; }

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            virtual void setAnchor(const AnchorStyle& anchor) { m_anchor = anchor; }
            virtual void setDockingOptions(const DockingOptions& options);
            virtual void setRawDockingSize(const Length& size);
            virtual void setRawTargetDockingSize(const Transition& targetSize);
            virtual void setDockingStyle(const DockingStyle& style);
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setNeedsDrawableReload(bool needsDrawableReload) { m_needsDrawableReload = needsDrawableReload; }
            virtual void setPositionType(const PositionType& positionType) { m_positionType = positionType; updateSpatialState(); }
            virtual void setPosition(const f32v2& position, bool update = true);
            virtual void setRawPosition(const Length2& rawPosition) { m_rawPosition = rawPosition; updatePositionState(); }
            virtual void setRawPosition(const f32v2& rawPosition, UnitType& units);
            virtual void setRawPositionX(f32 value, UnitType& units) { m_rawPosition.x = value; m_rawPosition.units.x = units; updatePositionState(); }
            virtual void setRawPositionY(f32 value, UnitType& units) { m_rawPosition.y = value; m_rawPosition.units.y = units; updatePositionState(); }
            virtual void setTargetRawPosition(const Transition2& targetRawPosition);
            virtual void setDimensions(const f32v2& dimensions);
            virtual void setRawDimensions(const Length2& rawDimensions) { m_rawDimensions = rawDimensions; updateDimensionState(); }
            virtual void setRawDimensions(const f32v2& rawDimensions, UnitType& units);
            virtual void setRawWidth(f32 value, UnitType& units) { m_rawDimensions.x = value; m_rawDimensions.units.x = units; updateDimensionState(); }
            virtual void setRawHeight(f32 value, UnitType& units) { m_rawDimensions.y = value; m_rawDimensions.units.y = units; updateDimensionState(); }
            virtual void setTargetRawDimensions(const Transition2& targetRawDimensions);
            virtual void setRawMaxSize(const Length2& maxSize) { m_rawMaxSize = maxSize; updateMaxSize(); }
            virtual void setRawMaxSize(const f32v2& maxSize, UnitType& units);
            virtual void setRawMaxWidth(f32 maxWidth, UnitType& units) { m_rawMaxSize.x = maxWidth; m_rawMaxSize.units.x = units; updateMaxSize(); }
            virtual void setRawMaxHeight(f32 maxHeight, UnitType& units) { m_rawMaxSize.y = maxHeight; m_rawMaxSize.units.y = units; updateMaxSize(); }
            virtual void setMaxSize(const f32v2& maxSize);
            virtual void setTargetRawMaxSize(const Transition2& targetMaxSize);
            virtual void setRawMinSize(const Length2& minSize) { m_rawMinSize = minSize; updateMinSize(); }
            virtual void setRawMinSize(const f32v2& minSize, UnitType& units);
            virtual void setRawMinWidth(f32 minWidth, UnitType& units) { m_rawMinSize.x = minWidth; m_rawMinSize.units.x = units; updateMinSize(); }
            virtual void setRawMinHeight(f32 minHeight, UnitType& units) { m_rawMinSize.y = minHeight; m_rawMinSize.units.y = units; updateMinSize(); }
            virtual void setMinSize(const f32v2& minSize);
            virtual void setTargetRawMinSize(const Transition2& targetMinSize);
            virtual void setWidgetAlign(WidgetAlign align) { m_align = align; updatePositionState(); }
            virtual void setZIndex(ui16 zIndex);
            
        protected:
            virtual f32v2 getWidgetAlignOffset();

            /*! @brief Updates the position relative to parent */
            virtual void updatePosition() override;
            /*! @brief Updates the dimensions of the widget based on processed positioning and size boundaries. */
            virtual void updateDimensions() override;
            /*! @brief Processes the raw maximum size then updates the dimensions appropriately. */
            virtual void updateMaxSize() { m_maxSize = processRawValues(m_rawMaxSize); updateDimensionState(); }
            /*! @brief Processes the raw minimum size then updates the dimensions appropriately. */
            virtual void updateMinSize() { m_minSize = processRawValues(m_rawMinSize); updateDimensionState(); }
            /*! @brief Processes the raw size of docking. */
            virtual void updateDockingSize();

            /*! @brief Processes a set of raw values and converts them to processed values that can be used for basic calculations. */
            virtual f32v2 processRawValues(const Length2& rawValues);
            virtual f32v2 processRawValue(const f32v2& rawValue, const UnitType& unit);

            /*! @brief Calculate relatve-to-parent change in position. */
            virtual f32v2 calculateRelativeToParentShift();

            /*! @brief Applies the current min and max sizes to the provided dimensions. */
            virtual void applyMinMaxSizesToDimensions(f32v2& dimensions);
            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            WidgetAlign m_align = WidgetAlign::TOP_LEFT; ///< Direction of alignment of the widget.
            AnchorStyle m_anchor; ///< The anchor data.
            DockingOptions m_dockingOptions; ///< Docking options of the widget.
            Transition m_targetDockingSize; ///< The target docking size of the widget.
            f32 m_processedDockingSize; ///< Cache of processed docking size.
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            UIRenderer* m_renderer = nullptr; ///< The renderer to be used by the widget.
            ui16 m_zIndex; ///< The Z-index of the widget.
            PositionType m_positionType = PositionType::STATIC; ///< The type of positioning of the widget.
            Length2 m_rawPosition; ///< The raw position of the widget.
            Transition2 m_targetRawPosition; ///< The target raw position of the widget.
            // TODO(Matthew): Do we need this?
            f32v2 m_relativePosition = f32v2(0.0f); ///< The relative position of the widget.
            Length2 m_rawDimensions; ///< The raw dimensions of the widget.
            Transition2 m_targetRawDimensions; ///< The target raw dimensions of the widget.
            Length2 m_rawMinSize; ///< The raw minimum dimensions of the widget.
            Transition2 m_targetRawMinSize; ///< The target raw minimum dimensions of the widget.
            f32v2 m_minSize = f32v2(0.0f); ///< The processed minimum dimensions of the widget.
            Length2 m_rawMaxSize; ///< The raw maximum dimensions of the widget.
            Transition2 m_targetRawMaxSize; ///< The target raw minimum dimensions of the widget.
            f32v2 m_maxSize = f32v2(FLT_MAX); ///< The processed maximum dimensions of the widget.
            volatile bool m_needsDrawableReload = false;
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
