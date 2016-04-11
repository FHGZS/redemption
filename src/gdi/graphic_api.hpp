/*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*   Product name: redemption, a FLOSS RDP proxy
*   Copyright (C) Wallix 2010-2015
*   Author(s): Jonathan Poelen
*/

#ifndef REDEMPTION_GDI_GRAPHIC_DEVICE_HPP
#define REDEMPTION_GDI_GRAPHIC_DEVICE_HPP

#include <type_traits>

#include <cstdint>

#include "utils/noncopyable.hpp"


class BGRPalette;
class RDPDestBlt;
class RDPMultiDstBlt;
class RDPPatBlt;
class RDPOpaqueRect;
class RDPMultiOpaqueRect;
class RDPScrBlt;
class RDPMemBlt;
class RDPMem3Blt;
class RDPLineTo;
class RDPGlyphIndex;
class RDPPolygonSC;
class RDPPolygonCB;
class RDPPolyline;
class RDPEllipseSC;
class RDPEllipseCB;

class RDPBitmapData;
class Pointer;
class Rect;
class Bitmap;
class GlyphCache;

class RDPColCache;
class RDPBrushCache;

namespace RDP {
    class RDPMultiPatBlt;
    class RDPMultiScrBlt;
    class FrameMarker;

    namespace RAIL {
        class NewOrExistingWindow;
        class WindowIcon;
        class CachedIcon;
        class DeletedWindow;
    }
}

namespace gdi {

struct GraphicDepth
{
    static constexpr GraphicDepth unspecified() { return {0}; }
    static constexpr GraphicDepth depth8() { return {1}; }
    static constexpr GraphicDepth depth15() { return {2}; }
    static constexpr GraphicDepth depth16() { return {3}; }
    static constexpr GraphicDepth depth24() { return {4}; }

    static constexpr GraphicDepth from_bpp(uint8_t bpp) {
        return {
            bpp == 8  ? depth8() :
            bpp == 15 ? depth15() :
            bpp == 16 ? depth16() :
            bpp == 24 ? depth24() :
            bpp == 32 ? depth24() :
            unspecified()
        };
    }

    constexpr GraphicDepth(GraphicDepth const &) = default;

private:
    struct bpp_table { int table[5] = {0, 8, 15, 16, 24}; };
public:
    constexpr uint8_t to_bpp(uint8_t bpp) const {
        return bpp_table{}.table[unsigned(this->depth_)];
    }

    constexpr bool is_defined() const { return !this->is_unspecified(); }
    constexpr bool is_unspecified() const { return this->depth_ == unspecified().depth_; }
    constexpr bool is_depth8() const { return this->depth_ == depth8().depth_; }
    constexpr bool is_depth15() const { return this->depth_ == depth15().depth_; }
    constexpr bool is_depth16() const { return this->depth_ == depth16().depth_; }
    constexpr bool is_depth24() const { return this->depth_ == depth24().depth_; }

    constexpr bool contains(GraphicDepth depth) const {
        return this->is_unspecified() || depth.is_unspecified() || (this->depth_ == depth.depth_);
    }

    constexpr GraphicDepth const & depth_or(GraphicDepth const & default_depth) const {
        return this->is_unspecified() ? default_depth : *this;
    }

    constexpr unsigned id() const { return this->depth_; }

private:
    enum class PrivateDepth { d0, d1, d2, d3, d4 };
public:
    // for switch/case, == and !=
    constexpr operator PrivateDepth () const { return static_cast<PrivateDepth>(this->depth_); }

private:
    constexpr GraphicDepth(uint8_t depth) : depth_(depth) {}

    uint8_t depth_;
};


struct GraphicApi : private noncopyable
{
    GraphicApi(GraphicDepth const & order_depths = GraphicDepth::unspecified())
    : order_depth_(order_depths)
    {}

    virtual ~GraphicApi() = default;

    virtual void set_pointer(Pointer      const & cursor) {}
    virtual void set_palette(BGRPalette   const & palette) {}

    virtual void draw(RDP::FrameMarker    const & order) = 0;
    virtual void draw(RDPDestBlt          const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPMultiDstBlt      const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPPatBlt           const & cmd, Rect const & clip) = 0;
    virtual void draw(RDP::RDPMultiPatBlt const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPOpaqueRect       const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPMultiOpaqueRect  const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPScrBlt           const & cmd, Rect const & clip) = 0;
    virtual void draw(RDP::RDPMultiScrBlt const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPLineTo           const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPPolygonSC        const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPPolygonCB        const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPPolyline         const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPEllipseSC        const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPEllipseCB        const & cmd, Rect const & clip) = 0;
    virtual void draw(RDPBitmapData       const & cmd, Bitmap const & bmp) = 0;
    virtual void draw(RDPMemBlt           const & cmd, Rect const & clip, Bitmap const & bmp) = 0;
    virtual void draw(RDPMem3Blt          const & cmd, Rect const & clip, Bitmap const & bmp) = 0;
    virtual void draw(RDPGlyphIndex       const & cmd, Rect const & clip, GlyphCache const & gly_cache) = 0;

    // NOTE maybe in an other interface
    virtual void draw(const RDP::RAIL::NewOrExistingWindow &) {}
    virtual void draw(const RDP::RAIL::WindowIcon          &) {}
    virtual void draw(const RDP::RAIL::CachedIcon          &) {}
    virtual void draw(const RDP::RAIL::DeletedWindow       &) {}

    // TODO("The 2 methods below should not exist and cache access be done before calling drawing orders")
    virtual void draw(RDPColCache   const & cmd) {}
    virtual void draw(RDPBrushCache const & cmd) {}

    virtual void sync() {}

    // TODO berk, data within size
    virtual void set_row(std::size_t rownum, const uint8_t * data) {}

    GraphicDepth const & order_depth() const {
        return this->order_depth_;
    }

private:
    GraphicDepth order_depth_;

protected:
    void set_depths(GraphicDepth const & depths) {
        this->order_depth_ = depths;
    }
};


struct GraphicCoreAccess
{
    template<class Derived, class... Ts>
    static void draw(Derived & derived, Ts const & ... args) {
        derived.draw_impl(args...);
    }

    template<class Derived>
    static auto color_converter(Derived const & derived)
    -> decltype(derived.color_converter_impl()) {
        return derived.color_converter_impl();
    }

    template<class Derived, class Tag, class... Ts>
    static void invoke_uniform_gd(Derived & derived, Tag tag, Ts const & ... args) {
        derived.invoke_uniform_gd_impl(tag, args...);
    }

    template<class Derived>
    static auto get_gd_proxy(Derived & derived)
    -> decltype(derived.get_gd_proxy_impl()) {
        return derived.get_gd_proxy_impl();
    }

    template<class Derived>
    static auto get_gd_list(Derived & derived)
    -> decltype(derived.get_gd_list_impl()) {
        return derived.get_gd_list_impl();
    }

    template<class Derived, class Gd>
    static auto to_gd_proxy(Derived & derived, Gd & gd)
    -> decltype(derived.to_gd_proxy_impl(gd)) {
        return derived.to_gd_proxy_impl(gd);
    }
};


/**
 * \code
 * struct Gd : gdi::GraphicBase<Gd>
 * {
 * private:
 *   friend gdi::GraphicCoreAccess;
 *
 *   template<class Cmd, class... Args>
 *   void draw_impl(Cmd const & cmd, Args const & ...) { log(cmd_name(cmd)); }
 * };
 * \endcode
 */
template<class Derived, class InterfaceBase = GraphicApi, class CoreAccess = GraphicCoreAccess>
class GraphicBase : public InterfaceBase
{
    static_assert(std::is_base_of<GraphicApi, InterfaceBase>::value, "InterfaceBase isn't a GraphicApi");

    friend CoreAccess;

protected:
    using core_access = CoreAccess;
    using base_type = GraphicBase;

public:
    using InterfaceBase::InterfaceBase;

    void draw(RDP::FrameMarker    const & order) override { this->draw_(order); }

    void draw(RDPDestBlt          const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPMultiDstBlt      const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPPatBlt           const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDP::RDPMultiPatBlt const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPOpaqueRect       const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPMultiOpaqueRect  const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPScrBlt           const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDP::RDPMultiScrBlt const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPLineTo           const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPPolygonSC        const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPPolygonCB        const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPPolyline         const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPEllipseSC        const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }
    void draw(RDPEllipseCB        const & cmd, Rect const & clip) override { this->draw_(cmd, clip); }

    void draw(RDPBitmapData       const & cmd, Bitmap const & bmp) override { this->draw_(cmd, bmp); }

    void draw(RDPMemBlt           const & cmd, Rect const & clip, Bitmap const & bmp) override {
        this->draw_(cmd, clip, bmp);
    }

    void draw(RDPMem3Blt          const & cmd, Rect const & clip, Bitmap const & bmp) override {
        this->draw_(cmd, clip, bmp);
    }

    void draw(RDPGlyphIndex       const & cmd, Rect const & clip, GlyphCache const & gly_cache) override {
        this->draw_(cmd, clip, gly_cache);
    }

    void draw(const RDP::RAIL::NewOrExistingWindow & order) { this->draw_(order); }
    void draw(const RDP::RAIL::WindowIcon          & order) { this->draw_(order); }
    void draw(const RDP::RAIL::CachedIcon          & order) { this->draw_(order); }
    void draw(const RDP::RAIL::DeletedWindow       & order) { this->draw_(order); }

    void draw(RDPColCache   const & cmd) override { this->draw_(cmd); }
    void draw(RDPBrushCache const & cmd) override { this->draw_(cmd); }

protected:
    Derived & derived() {
        return static_cast<Derived&>(*this);
    }

private:
    template<class... Ts>
    void draw_(Ts const & ... args) {
        CoreAccess::draw(this->derived(), args...);
    }
};


/**
 * \code
 * struct Gd : gdi::GraphicProxy<Gd>
 * {
 * private:
 *   gdi::GraphicApi & internal;
 *
 *   friend gdi::GraphicCoreAccess;
 *
 *   gdi::GraphicApi & get_gd_proxy_impl() { return this->internal; }
 * };
 * \endcode
 */
template<class Derived, class InterfaceBase = GraphicApi, class CoreAccess = GraphicCoreAccess>
struct GraphicProxy : GraphicBase<Derived, InterfaceBase, CoreAccess>
{
    using GraphicBase<Derived, InterfaceBase, CoreAccess>::GraphicBase;
    using base_type = GraphicProxy;
    friend CoreAccess;

    void set_pointer(Pointer    const & pointer) override {
        CoreAccess::get_gd_proxy(this->derived()).set_pointer(pointer);
    }

    void set_palette(BGRPalette const & palette) override {
        CoreAccess::get_gd_proxy(this->derived()).set_palette(palette);
    }

    void sync() override {
        CoreAccess::get_gd_proxy(this->derived()).sync();
    }

    void set_row(std::size_t rownum, const uint8_t * data) override {
        CoreAccess::get_gd_proxy(this->derived()).set_row(rownum, data);
    }

protected:
    template<class... Ts>
    void draw_impl(Ts const & ... args) {
        CoreAccess::get_gd_proxy(this->derived()).draw(args...);
    }
};


/**
 * \code
 * struct Gd : gdi::GraphicDispatcher<Gd>
 * {
 * private:
 *   std::vector<std::reference_wrapper<gdi::GraphicApi>> gds;
 *
 *   friend gdi::GraphicCoreAccess;
 *
 *   auto & get_gd_list_impl() { return this->gds; }
 *
 *   //GraphicApiWrapper to_gd_proxy_impl(List::value_type & gd) { return wrap(gd); }
 * };
 * \endcode
 */
template<class Derived, class InterfaceBase = GraphicApi, class CoreAccess = GraphicCoreAccess>
struct GraphicDispatcher : GraphicBase<Derived, InterfaceBase, CoreAccess>
{
    using GraphicBase<Derived, InterfaceBase, CoreAccess>::GraphicBase;
    using base_type = GraphicDispatcher;
    friend CoreAccess;

    void set_pointer(Pointer    const & pointer) override {
        for (auto && gd : CoreAccess::get_gd_list(this->derived())) {
            CoreAccess::to_gd_proxy(this->derived(), gd).set_pointer(pointer);
        }
    }

    void set_palette(BGRPalette const & palette) override {
        for (auto && gd : CoreAccess::get_gd_list(this->derived())) {
            CoreAccess::to_gd_proxy(this->derived(), gd).set_palette(palette);
        }
    }

    void sync() override {
        for (auto && gd : CoreAccess::get_gd_list(this->derived())) {
            CoreAccess::to_gd_proxy(this->derived(), gd).sync();
        }
    }

    void set_row(std::size_t rownum, const uint8_t * data) override {
        for (auto && gd : CoreAccess::get_gd_list(this->derived())) {
            CoreAccess::to_gd_proxy(this->derived(), gd).set_row(rownum, data);
        }
    }

protected:
    template<class... Ts>
    void draw_impl(Ts const & ... args) {
        for (auto && gd : CoreAccess::get_gd_list(this->derived())) {
            CoreAccess::to_gd_proxy(this->derived(), gd).draw(args...);
        }
    }

    GraphicApi & to_gd_proxy_impl(GraphicApi & gd) {
        return gd;
    }
};


/**
 * \code
 * struct Gd : gdi::GraphicUniFormProxy<Gd>
 * {
 * private:
 *   friend gdi::GraphicCoreAccess;
 *
 *   template<class Cmd, class... Args>
 *   void invoke_uniform_gd_impl(draw_tag, Cmd const & cmd, Args const & ... args) { ... }
 *
 *   void invoke_uniform_gd_impl(set_tag, BGRPalette const & palette) { ... }
 *   void invoke_uniform_gd_impl(set_tag, Pointer    const & pointer) { ... }
 *
 *   void invoke_uniform_gd_impl(sync_tag) { ... }
 *
 *   void invoke_uniform_gd_impl(set_row_tag, std::size_t rownum, const uint8_t * data) { ... }
 * };
 * \endcode
 */
template<class Derived, class InterfaceBase = GraphicApi, class CoreAccess = GraphicCoreAccess>
struct GraphicUniFormProxy : GraphicBase<Derived, InterfaceBase, CoreAccess>
{
    using GraphicBase<Derived, InterfaceBase, CoreAccess>::GraphicBase;
    using base_type = GraphicUniFormProxy;
    friend CoreAccess;

    struct draw_tag {};
    struct set_tag {};
    struct sync_tag {};
    struct set_row_tag {};

    void set_pointer(Pointer    const & pointer) override {
        CoreAccess::invoke_uniform_gd(this->derived(), set_tag{}, pointer);
    }

    void set_palette(BGRPalette const & palette) override {
        CoreAccess::invoke_uniform_gd(this->derived(), set_tag{}, palette);
    }

    void sync() override {
        CoreAccess::invoke_uniform_gd(this->derived(), sync_tag{});
    }

    void set_row(std::size_t rownum, const uint8_t * data) override {
        CoreAccess::invoke_uniform_gd(this->derived(), set_row_tag{}, rownum, data);
    }

protected:
    template<class... Ts> void draw_impl(Ts const & ... args) {
        CoreAccess::invoke_uniform_gd(this->derived(), draw_tag{}, args...);
    }
};


class BlackoutGraphic : public GraphicBase<BlackoutGraphic>
{
    friend gdi::GraphicCoreAccess;

    template<class... Args>
    void draw_impl(Args const & ...) {}
};

}

#endif
