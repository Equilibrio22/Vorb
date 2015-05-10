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

DECL_VG(class SpriteFont)

namespace vorb {
    namespace ui {

        // Forward Declarations
        struct MouseButtonEvent;
        struct MouseMotionEvent;
        struct MouseEvent;
        class UIRenderer;

        //! Bitfield of control styling flags
        struct ControlStyle {
            bool fixedHeight : 1; ///< If true, the control has fixed height when auto-scaled.
            bool fixedWidth : 1; ///< If true, the control has fixed width when auto-scaled.
            bool selectable : 1; ///< If true, the control can receive focus.
        };
        //! Bitfield of anchor flags
        struct AnchorStyle {
            bool left : 1; ///< If true, anchored to the left of parent
            bool right : 1; ///< If true, anchored to the right of parent
            bool bottom : 1; ///< If true, anchored to the bottom of parent
            bool top : 1; ///< If true, anchored to the top of parent
        };
        //! Bitfield of dock flags
        enum class DockStyle {
            NONE, LEFT, RIGHT, BOTTOM, TOP, FILL
        };

        class Widget {
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
             * The control will be made a child of parent.
             *
             * @param parent: Parent control object.
             * @param name: Name of the control.
             * @param destRect: Rectangle defining the position and dimensions as the tuple <x,y,w,h>.
             */
            Widget(Widget* parent, const nString& name, const f32v4& destRect = f32v4(0));
            /*! @brief Destructor that unhooks events */
            virtual ~Widget();

            /*! @brief Releases all resources used by the Control.
             * 
             * Gets called in the destructor.
             */
            virtual void dispose();

            /*! @brief Adds a child to the control
             *
             * @param child: The control to add
             * @return true on success.
             */
            virtual bool addChild(Widget* child);

            /*! @brief Adds all drawables to the UIRenderer
            *
            * @param renderer: UIRenderer to add to
            */
            virtual void addDrawables(UIRenderer* renderer) { /* Empty */ }

            /*! @brief Removes all drawables from the UIRenderer
            *
            * @param renderer: UIRenderer to remove from
            */
            virtual void removeDrawables(UIRenderer* renderer) { /* Empty */ }

            /*! @brief Updates the position relative to parent */
            virtual void updatePosition();

            /*! @brief Enables events* */
            virtual void enable(); 

            /*! @brief Disables events* */
            virtual void disable();

            /*! @brief Checks if a point is inside the widget
            *
            * @param point: The point to check
            * @return true if point is in m_destRect
            */
            virtual bool isInBounds(const f32v2& point) { return isInBounds(point.x, point.y); }
            virtual bool isInBounds(f32 x, f32 y);

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            virtual bool getFixedHeight() const { return m_style.fixedHeight; }
            virtual bool getFixedWidth() const { return m_style.fixedWidth; }
            virtual bool getSelectable() const { return m_style.selectable; }
            virtual bool isMouseIn() const { return m_isMouseIn; }
            virtual const AnchorStyle& getAnchor() const { return m_anchor; }
            virtual const ControlStyle& getStyle() const { return m_style; }
            virtual const DockStyle& getDock() const { return m_dock; }
            virtual const bool& isRenderInit() const { return m_isRenderInit; }
            virtual const bool& isEnabled() const { return m_isEnabled; }
            virtual const f32& getHeight() const { return m_dimensions.y; }
            virtual const f32& getWidth() const { return m_dimensions.x; }
            virtual const f32& getX() const { return m_position.x; }
            virtual const f32& getY() const { return m_position.y; }
            virtual const f32v2& getDimensions() const { return m_dimensions; }
            virtual const f32v2& getPosition() const { return m_position; }
            virtual const f32v2& getRelativePosition() const { return m_relativePosition; }
            virtual const std::vector<Widget*>& getWidgets() const { return m_widgets; }
            virtual const vorb::graphics::SpriteFont* getFont() const { return m_font; }
            
            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            // TODO(Ben): Propagate changes to children
            virtual void setAnchor(const AnchorStyle& anchor) { m_anchor = anchor; }
            virtual void setDestRect(const f32v4& destRect);
            virtual void setDimensions(const f32v2& dimensions) { m_dimensions = dimensions; }
            virtual void setDock(const DockStyle& dock) { m_dock = dock; }
            virtual void setFixedHeight(bool fixedHeight) { m_style.fixedHeight = fixedHeight; }
            virtual void setFixedWidth(bool fixedWidth) { m_style.fixedWidth = fixedWidth; }
            virtual void setFont(const vorb::graphics::SpriteFont* font) { m_font = font; }
            virtual void setHeight(f32 height) { m_dimensions.y = height; }
            virtual void setPosition(const f32v2& position) { m_relativePosition = position; updatePosition(); }
            virtual void setSelectable(bool selectable) { m_style.selectable = selectable; }
            virtual void setStyle(const ControlStyle& style) { m_style = style; }
            virtual void setWidth(f32 width) { m_dimensions.x = width; }
            virtual void setX(f32 x) { m_relativePosition.x = x; updatePosition(); }
            virtual void setY(f32 y) { m_relativePosition.y = y; updatePosition(); }

            /************************************************************************/
            /* Events                                                               */
            /************************************************************************/
            Event<const MouseButtonEvent&> MouseClick; ///< Occurs when control is clicked by mouse.
            Event<const MouseButtonEvent&> MouseDown; ///< Occurs when mouse button is pressed over control.
            Event<const MouseButtonEvent&> MouseUp; ///< Occurs when mouse button is released over control.
            Event<const MouseMotionEvent&> MouseEnter; ///< Occurs when mouse pointer enters the control.
            Event<const MouseMotionEvent&> MouseLeave; ///< Occurs when mouse pointer leaves the control.
            Event<const MouseMotionEvent&> MouseMove; ///< Occurs when mouse pointer is moved over control.
            // TODO(Ben): Lots more events!

        protected:
            virtual void setParent(Widget* parent) { m_parent = parent; }
            /************************************************************************/
            /* Event Handlers                                                       */
            /************************************************************************/
            virtual void onMouseDown(Sender s, const MouseButtonEvent& e);
            virtual void onMouseUp(Sender s, const MouseButtonEvent& e);
            virtual void onMouseMove(Sender s, const MouseMotionEvent& e);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            Widget* m_parent = nullptr; ///< Widget that this widget is relative to
            const vorb::graphics::SpriteFont* m_font = nullptr; ///< Font for rendering.
            AnchorStyle m_anchor; ///< The anchor data.
            ControlStyle m_style; ///< The current style.
            DockStyle m_dock; ///< The dock type.
            std::vector<Widget*> m_widgets; ///< All child widgets.
            f32v2 m_relativePosition = f32v2(0.0f); ///< Position relative to parent
            f32v2 m_position = f32v2(0.0f); ///< The position and dimensions.
            f32v2 m_dimensions = f32v2(0.0f); ///< The position and dimensions.
            nString m_name = ""; ///< Display name of the control

            // TODO(Ben): Bitfield for memory reduction?
            bool m_isRenderInit = false; ///< True when rendering is initialized.
            bool m_isEnabled = false; ///< True when events are enabled
            bool m_isClicking = false; ///< Used for click event tracking
            bool m_isMouseIn = false; ///< Used for motion event tracking
        };
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_Widget_h__
