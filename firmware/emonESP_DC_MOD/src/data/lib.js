/*! jQuery v2.1.1 -css,-css/addGetHookIf,-css/curCSS,-css/defaultDisplay,-css/hiddenVisibleSelectors,-css/support,-css/swap,-css/var/cssExpand,-css/var/getStyles,-css/var/isHidden,-css/var/rmargin,-css/var/rnumnonpx,-effects,-effects/Tween,-effects/animatedSelector,-dimensions,-offset,-deprecated,-event-alias,-wrap | (c) 2005, 2014 jQuery Foundation, Inc. | jquery.org/license */ ! function(a, b) {
    "object" == typeof module && "object" == typeof module.exports ? module.exports = a.document ? b(a, !0) : function(a) {
        if (!a.document) throw new Error("jQuery requires a window with a document");
        return b(a)
    } : b(a)
}("undefined" != typeof window ? window : this, function(a, b) {
    var c = [],
        d = c.slice,
        e = c.concat,
        f = c.push,
        g = c.indexOf,
        h = {},
        i = h.toString,
        j = h.hasOwnProperty,
        k = {},
        l = a.document,
        m = "2.1.1 -css,-css/addGetHookIf,-css/curCSS,-css/defaultDisplay,-css/hiddenVisibleSelectors,-css/support,-css/swap,-css/var/cssExpand,-css/var/getStyles,-css/var/isHidden,-css/var/rmargin,-css/var/rnumnonpx,-effects,-effects/Tween,-effects/animatedSelector,-dimensions,-offset,-deprecated,-event-alias,-wrap",
        n = function(a, b) {
            return new n.fn.init(a, b)
        },
        o = /^[\s\uFEFF\xA0]+|[\s\uFEFF\xA0]+$/g,
        p = /^-ms-/,
        q = /-([\da-z])/gi,
        r = function(a, b) {
            return b.toUpperCase()
        };
    n.fn = n.prototype = {
        jquery: m,
        constructor: n,
        selector: "",
        length: 0,
        toArray: function() {
            return d.call(this)
        },
        get: function(a) {
            return null != a ? 0 > a ? this[a + this.length] : this[a] : d.call(this)
        },
        pushStack: function(a) {
            var b = n.merge(this.constructor(), a);
            return b.prevObject = this, b.context = this.context, b
        },
        each: function(a, b) {
            return n.each(this, a, b)
        },
        map: function(a) {
            return this.pushStack(n.map(this, function(b, c) {
                return a.call(b, c, b)
            }))
        },
        slice: function() {
            return this.pushStack(d.apply(this, arguments))
        },
        first: function() {
            return this.eq(0)
        },
        last: function() {
            return this.eq(-1)
        },
        eq: function(a) {
            var b = this.length,
                c = +a + (0 > a ? b : 0);
            return this.pushStack(c >= 0 && b > c ? [this[c]] : [])
        },
        end: function() {
            return this.prevObject || this.constructor(null)
        },
        push: f,
        sort: c.sort,
        splice: c.splice
    }, n.extend = n.fn.extend = function() {
        var a, b, c, d, e, f, g = arguments[0] || {},
            h = 1,
            i = arguments.length,
            j = !1;
        for ("boolean" == typeof g && (j = g, g = arguments[h] || {}, h++), "object" == typeof g || n.isFunction(g) || (g = {}), h === i && (g = this, h--); i > h; h++)
            if (null != (a = arguments[h]))
                for (b in a) c = g[b], d = a[b], g !== d && (j && d && (n.isPlainObject(d) || (e = n.isArray(d))) ? (e ? (e = !1, f = c && n.isArray(c) ? c : []) : f = c && n.isPlainObject(c) ? c : {}, g[b] = n.extend(j, f, d)) : void 0 !== d && (g[b] = d));
        return g
    }, n.extend({
        expando: "jQuery" + (m + Math.random()).replace(/\D/g, ""),
        isReady: !0,
        error: function(a) {
            throw new Error(a)
        },
        noop: function() {},
        isFunction: function(a) {
            return "function" === n.type(a)
        },
        isArray: Array.isArray,
        isWindow: function(a) {
            return null != a && a === a.window
        },
        isNumeric: function(a) {
            return !n.isArray(a) && a - parseFloat(a) >= 0
        },
        isPlainObject: function(a) {
            return "object" !== n.type(a) || a.nodeType || n.isWindow(a) ? !1 : a.constructor && !j.call(a.constructor.prototype, "isPrototypeOf") ? !1 : !0
        },
        isEmptyObject: function(a) {
            var b;
            for (b in a) return !1;
            return !0
        },
        type: function(a) {
            return null == a ? a + "" : "object" == typeof a || "function" == typeof a ? h[i.call(a)] || "object" : typeof a
        },
        globalEval: function(a) {
            var b, c = eval;
            a = n.trim(a), a && (1 === a.indexOf("use strict") ? (b = l.createElement("script"), b.text = a, l.head.appendChild(b).parentNode.removeChild(b)) : c(a))
        },
        camelCase: function(a) {
            return a.replace(p, "ms-").replace(q, r)
        },
        nodeName: function(a, b) {
            return a.nodeName && a.nodeName.toLowerCase() === b.toLowerCase()
        },
        each: function(a, b, c) {
            var d, e = 0,
                f = a.length,
                g = s(a);
            if (c) {
                if (g) {
                    for (; f > e; e++)
                        if (d = b.apply(a[e], c), d === !1) break
                } else
                    for (e in a)
                        if (d = b.apply(a[e], c), d === !1) break
            } else if (g) {
                for (; f > e; e++)
                    if (d = b.call(a[e], e, a[e]), d === !1) break
            } else
                for (e in a)
                    if (d = b.call(a[e], e, a[e]), d === !1) break;
            return a
        },
        trim: function(a) {
            return null == a ? "" : (a + "").replace(o, "")
        },
        makeArray: function(a, b) {
            var c = b || [];
            return null != a && (s(Object(a)) ? n.merge(c, "string" == typeof a ? [a] : a) : f.call(c, a)), c
        },
        inArray: function(a, b, c) {
            return null == b ? -1 : g.call(b, a, c)
        },
        merge: function(a, b) {
            for (var c = +b.length, d = 0, e = a.length; c > d; d++) a[e++] = b[d];
            return a.length = e, a
        },
        grep: function(a, b, c) {
            for (var d, e = [], f = 0, g = a.length, h = !c; g > f; f++) d = !b(a[f], f), d !== h && e.push(a[f]);
            return e
        },
        map: function(a, b, c) {
            var d, f = 0,
                g = a.length,
                h = s(a),
                i = [];
            if (h)
                for (; g > f; f++) d = b(a[f], f, c), null != d && i.push(d);
            else
                for (f in a) d = b(a[f], f, c), null != d && i.push(d);
            return e.apply([], i)
        },
        guid: 1,
        proxy: function(a, b) {
            var c, e, f;
            return "string" == typeof b && (c = a[b], b = a, a = c), n.isFunction(a) ? (e = d.call(arguments, 2), f = function() {
                return a.apply(b || this, e.concat(d.call(arguments)))
            }, f.guid = a.guid = a.guid || n.guid++, f) : void 0
        },
        now: Date.now,
        support: k
    }), n.each("Boolean Number String Function Array Date RegExp Object Error".split(" "), function(a, b) {
        h["[object " + b + "]"] = b.toLowerCase()
    });

    function s(a) {
        var b = a.length,
            c = n.type(a);
        return "function" === c || n.isWindow(a) ? !1 : 1 === a.nodeType && b ? !0 : "array" === c || 0 === b || "number" == typeof b && b > 0 && b - 1 in a
    }
    var t = a.document.documentElement,
        u, v = t.matches || t.webkitMatchesSelector || t.mozMatchesSelector || t.oMatchesSelector || t.msMatchesSelector,
        w = function(a, b) {
            if (a === b) return u = !0, 0;
            var c = b.compareDocumentPosition && a.compareDocumentPosition && a.compareDocumentPosition(b);
            return c ? 1 & c ? a === l || n.contains(l, a) ? -1 : b === l || n.contains(l, b) ? 1 : 0 : 4 & c ? -1 : 1 : a.compareDocumentPosition ? -1 : 1
        };
    n.extend({
        find: function(a, b, c, d) {
            var e, f, g = 0;
            if (c = c || [], b = b || l, !a || "string" != typeof a) return c;
            if (1 !== (f = b.nodeType) && 9 !== f) return [];
            if (d)
                while (e = d[g++]) n.find.matchesSelector(e, a) && c.push(e);
            else n.merge(c, b.querySelectorAll(a));
            return c
        },
        unique: function(a) {
            var b, c = [],
                d = 0,
                e = 0;
            if (u = !1, a.sort(w), u) {
                while (b = a[d++]) b === a[d] && (e = c.push(d));
                while (e--) a.splice(c[e], 1)
            }
            return a
        },
        text: function(a) {
            var b, c = "",
                d = 0,
                e = a.nodeType;
            if (e) {
                if (1 === e || 9 === e || 11 === e) return a.textContent;
                if (3 === e || 4 === e) return a.nodeValue
            } else
                while (b = a[d++]) c += n.text(b);
            return c
        },
        contains: function(a, b) {
            var c = 9 === a.nodeType ? a.documentElement : a,
                d = b && b.parentNode;
            return a === d || !(!d || 1 !== d.nodeType || !c.contains(d))
        },
        isXMLDoc: function(a) {
            return "HTML" !== (a.ownerDocument || a).documentElement.nodeName
        },
        expr: {
            attrHandle: {},
            match: {
                bool: /^(?:checked|selected|async|autofocus|autoplay|controls|defer|disabled|hidden|ismap|loop|multiple|open|readonly|required|scoped)$/i,
                needsContext: /^[\x20\t\r\n\f]*[>+~]/
            }
        }
    }), n.extend(n.find, {
        matches: function(a, b) {
            return n.find(a, null, null, b)
        },
        matchesSelector: function(a, b) {
            return v.call(a, b)
        },
        attr: function(a, b) {
            return a.getAttribute(b)
        }
    });
    var x = n.expr.match.needsContext,
        y = /^<(\w+)\s*\/?>(?:<\/\1>|)$/,
        z = /^.[^:#\[\.,]*$/;

    function A(a, b, c) {
        if (n.isFunction(b)) return n.grep(a, function(a, d) {
            return !!b.call(a, d, a) !== c
        });
        if (b.nodeType) return n.grep(a, function(a) {
            return a === b !== c
        });
        if ("string" == typeof b) {
            if (z.test(b)) return n.filter(b, a, c);
            b = n.filter(b, a)
        }
        return n.grep(a, function(a) {
            return g.call(b, a) >= 0 !== c
        })
    }
    n.filter = function(a, b, c) {
        var d = b[0];
        return c && (a = ":not(" + a + ")"), 1 === b.length && 1 === d.nodeType ? n.find.matchesSelector(d, a) ? [d] : [] : n.find.matches(a, n.grep(b, function(a) {
            return 1 === a.nodeType
        }))
    }, n.fn.extend({
        find: function(a) {
            var b, c = this.length,
                d = [],
                e = this;
            if ("string" != typeof a) return this.pushStack(n(a).filter(function() {
                for (b = 0; c > b; b++)
                    if (n.contains(e[b], this)) return !0
            }));
            for (b = 0; c > b; b++) n.find(a, e[b], d);
            return d = this.pushStack(c > 1 ? n.unique(d) : d), d.selector = this.selector ? this.selector + " " + a : a, d
        },
        filter: function(a) {
            return this.pushStack(A(this, a || [], !1))
        },
        not: function(a) {
            return this.pushStack(A(this, a || [], !0))
        },
        is: function(a) {
            return !!A(this, "string" == typeof a && x.test(a) ? n(a) : a || [], !1).length
        }
    });
    var B, C = /^(?:\s*(<[\w\W]+>)[^>]*|#([\w-]*))$/,
        D = n.fn.init = function(a, b) {
            var c, d;
            if (!a) return this;
            if ("string" == typeof a) {
                if (c = "<" === a[0] && ">" === a[a.length - 1] && a.length >= 3 ? [null, a, null] : C.exec(a), !c || !c[1] && b) return !b || b.jquery ? (b || B).find(a) : this.constructor(b).find(a);
                if (c[1]) {
                    if (b = b instanceof n ? b[0] : b, n.merge(this, n.parseHTML(c[1], b && b.nodeType ? b.ownerDocument || b : l, !0)), y.test(c[1]) && n.isPlainObject(b))
                        for (c in b) n.isFunction(this[c]) ? this[c](b[c]) : this.attr(c, b[c]);
                    return this
                }
                return d = l.getElementById(c[2]), d && d.parentNode && (this.length = 1, this[0] = d), this.context = l, this.selector = a, this
            }
            return a.nodeType ? (this.context = this[0] = a, this.length = 1, this) : n.isFunction(a) ? "undefined" != typeof B.ready ? B.ready(a) : a(n) : (void 0 !== a.selector && (this.selector = a.selector, this.context = a.context), n.makeArray(a, this))
        };
    D.prototype = n.fn, B = n(l);
    var E = /^(?:parents|prev(?:Until|All))/,
        F = {
            children: !0,
            contents: !0,
            next: !0,
            prev: !0
        };
    n.extend({
        dir: function(a, b, c) {
            var d = [],
                e = void 0 !== c;
            while ((a = a[b]) && 9 !== a.nodeType)
                if (1 === a.nodeType) {
                    if (e && n(a).is(c)) break;
                    d.push(a)
                } return d
        },
        sibling: function(a, b) {
            for (var c = []; a; a = a.nextSibling) 1 === a.nodeType && a !== b && c.push(a);
            return c
        }
    }), n.fn.extend({
        has: function(a) {
            var b = n(a, this),
                c = b.length;
            return this.filter(function() {
                for (var a = 0; c > a; a++)
                    if (n.contains(this, b[a])) return !0
            })
        },
        closest: function(a, b) {
            for (var c, d = 0, e = this.length, f = [], g = x.test(a) || "string" != typeof a ? n(a, b || this.context) : 0; e > d; d++)
                for (c = this[d]; c && c !== b; c = c.parentNode)
                    if (c.nodeType < 11 && (g ? g.index(c) > -1 : 1 === c.nodeType && n.find.matchesSelector(c, a))) {
                        f.push(c);
                        break
                    } return this.pushStack(f.length > 1 ? n.unique(f) : f)
        },
        index: function(a) {
            return a ? "string" == typeof a ? g.call(n(a), this[0]) : g.call(this, a.jquery ? a[0] : a) : this[0] && this[0].parentNode ? this.first().prevAll().length : -1
        },
        add: function(a, b) {
            return this.pushStack(n.unique(n.merge(this.get(), n(a, b))))
        },
        addBack: function(a) {
            return this.add(null == a ? this.prevObject : this.prevObject.filter(a))
        }
    });

    function G(a, b) {
        while ((a = a[b]) && 1 !== a.nodeType);
        return a
    }
    n.each({
        parent: function(a) {
            var b = a.parentNode;
            return b && 11 !== b.nodeType ? b : null
        },
        parents: function(a) {
            return n.dir(a, "parentNode")
        },
        parentsUntil: function(a, b, c) {
            return n.dir(a, "parentNode", c)
        },
        next: function(a) {
            return G(a, "nextSibling")
        },
        prev: function(a) {
            return G(a, "previousSibling")
        },
        nextAll: function(a) {
            return n.dir(a, "nextSibling")
        },
        prevAll: function(a) {
            return n.dir(a, "previousSibling")
        },
        nextUntil: function(a, b, c) {
            return n.dir(a, "nextSibling", c)
        },
        prevUntil: function(a, b, c) {
            return n.dir(a, "previousSibling", c)
        },
        siblings: function(a) {
            return n.sibling((a.parentNode || {}).firstChild, a)
        },
        children: function(a) {
            return n.sibling(a.firstChild)
        },
        contents: function(a) {
            return a.contentDocument || n.merge([], a.childNodes)
        }
    }, function(a, b) {
        n.fn[a] = function(c, d) {
            var e = n.map(this, b, c);
            return "Until" !== a.slice(-5) && (d = c), d && "string" == typeof d && (e = n.filter(d, e)), this.length > 1 && (F[a] || n.unique(e), E.test(a) && e.reverse()), this.pushStack(e)
        }
    });
    var H = /\S+/g,
        I = {};

    function J(a) {
        var b = I[a] = {};
        return n.each(a.match(H) || [], function(a, c) {
            b[c] = !0
        }), b
    }
    n.Callbacks = function(a) {
        a = "string" == typeof a ? I[a] || J(a) : n.extend({}, a);
        var b, c, d, e, f, g, h = [],
            i = !a.once && [],
            j = function(l) {
                for (b = a.memory && l, c = !0, g = e || 0, e = 0, f = h.length, d = !0; h && f > g; g++)
                    if (h[g].apply(l[0], l[1]) === !1 && a.stopOnFalse) {
                        b = !1;
                        break
                    } d = !1, h && (i ? i.length && j(i.shift()) : b ? h = [] : k.disable())
            },
            k = {
                add: function() {
                    if (h) {
                        var c = h.length;
                        ! function g(b) {
                            n.each(b, function(b, c) {
                                var d = n.type(c);
                                "function" === d ? a.unique && k.has(c) || h.push(c) : c && c.length && "string" !== d && g(c)
                            })
                        }(arguments), d ? f = h.length : b && (e = c, j(b))
                    }
                    return this
                },
                remove: function() {
                    return h && n.each(arguments, function(a, b) {
                        var c;
                        while ((c = n.inArray(b, h, c)) > -1) h.splice(c, 1), d && (f >= c && f--, g >= c && g--)
                    }), this
                },
                has: function(a) {
                    return a ? n.inArray(a, h) > -1 : !(!h || !h.length)
                },
                empty: function() {
                    return h = [], f = 0, this
                },
                disable: function() {
                    return h = i = b = void 0, this
                },
                disabled: function() {
                    return !h
                },
                lock: function() {
                    return i = void 0, b || k.disable(), this
                },
                locked: function() {
                    return !i
                },
                fireWith: function(a, b) {
                    return !h || c && !i || (b = b || [], b = [a, b.slice ? b.slice() : b], d ? i.push(b) : j(b)), this
                },
                fire: function() {
                    return k.fireWith(this, arguments), this
                },
                fired: function() {
                    return !!c
                }
            };
        return k
    }, n.extend({
        Deferred: function(a) {
            var b = [
                    ["resolve", "done", n.Callbacks("once memory"), "resolved"],
                    ["reject", "fail", n.Callbacks("once memory"), "rejected"],
                    ["notify", "progress", n.Callbacks("memory")]
                ],
                c = "pending",
                d = {
                    state: function() {
                        return c
                    },
                    always: function() {
                        return e.done(arguments).fail(arguments), this
                    },
                    then: function() {
                        var a = arguments;
                        return n.Deferred(function(c) {
                            n.each(b, function(b, f) {
                                var g = n.isFunction(a[b]) && a[b];
                                e[f[1]](function() {
                                    var a = g && g.apply(this, arguments);
                                    a && n.isFunction(a.promise) ? a.promise().done(c.resolve).fail(c.reject).progress(c.notify) : c[f[0] + "With"](this === d ? c.promise() : this, g ? [a] : arguments)
                                })
                            }), a = null
                        }).promise()
                    },
                    promise: function(a) {
                        return null != a ? n.extend(a, d) : d
                    }
                },
                e = {};
            return d.pipe = d.then, n.each(b, function(a, f) {
                var g = f[2],
                    h = f[3];
                d[f[1]] = g.add, h && g.add(function() {
                    c = h
                }, b[1 ^ a][2].disable, b[2][2].lock), e[f[0]] = function() {
                    return e[f[0] + "With"](this === e ? d : this, arguments), this
                }, e[f[0] + "With"] = g.fireWith
            }), d.promise(e), a && a.call(e, e), e
        },
        when: function(a) {
            var b = 0,
                c = d.call(arguments),
                e = c.length,
                f = 1 !== e || a && n.isFunction(a.promise) ? e : 0,
                g = 1 === f ? a : n.Deferred(),
                h = function(a, b, c) {
                    return function(e) {
                        b[a] = this, c[a] = arguments.length > 1 ? d.call(arguments) : e, c === i ? g.notifyWith(b, c) : --f || g.resolveWith(b, c)
                    }
                },
                i, j, k;
            if (e > 1)
                for (i = new Array(e), j = new Array(e), k = new Array(e); e > b; b++) c[b] && n.isFunction(c[b].promise) ? c[b].promise().done(h(b, k, c)).fail(g.reject).progress(h(b, j, i)) : --f;
            return f || g.resolveWith(k, c), g.promise()
        }
    });
    var K;
    n.fn.ready = function(a) {
        return n.ready.promise().done(a), this
    }, n.extend({
        isReady: !1,
        readyWait: 1,
        holdReady: function(a) {
            a ? n.readyWait++ : n.ready(!0)
        },
        ready: function(a) {
            (a === !0 ? --n.readyWait : n.isReady) || (n.isReady = !0, a !== !0 && --n.readyWait > 0 || (K.resolveWith(l, [n]), n.fn.triggerHandler && (n(l).triggerHandler("ready"), n(l).off("ready"))))
        }
    });

    function L() {
        l.removeEventListener("DOMContentLoaded", L, !1), a.removeEventListener("load", L, !1), n.ready()
    }
    n.ready.promise = function(b) {
        return K || (K = n.Deferred(), "complete" === l.readyState ? setTimeout(n.ready) : (l.addEventListener("DOMContentLoaded", L, !1), a.addEventListener("load", L, !1))), K.promise(b)
    }, n.ready.promise();
    var M = n.access = function(a, b, c, d, e, f, g) {
        var h = 0,
            i = a.length,
            j = null == c;
        if ("object" === n.type(c)) {
            e = !0;
            for (h in c) n.access(a, b, h, c[h], !0, f, g)
        } else if (void 0 !== d && (e = !0, n.isFunction(d) || (g = !0), j && (g ? (b.call(a, d), b = null) : (j = b, b = function(a, b, c) {
                return j.call(n(a), c)
            })), b))
            for (; i > h; h++) b(a[h], c, g ? d : d.call(a[h], h, b(a[h], c)));
        return e ? a : j ? b.call(a) : i ? b(a[0], c) : f
    };
    n.acceptData = function(a) {
        return 1 === a.nodeType || 9 === a.nodeType || !+a.nodeType
    };

    function N() {
        Object.defineProperty(this.cache = {}, 0, {
            get: function() {
                return {}
            }
        }), this.expando = n.expando + Math.random()
    }
    N.uid = 1, N.accepts = n.acceptData, N.prototype = {
        key: function(a) {
            if (!N.accepts(a)) return 0;
            var b = {},
                c = a[this.expando];
            if (!c) {
                c = N.uid++;
                try {
                    b[this.expando] = {
                        value: c
                    }, Object.defineProperties(a, b)
                } catch (d) {
                    b[this.expando] = c, n.extend(a, b)
                }
            }
            return this.cache[c] || (this.cache[c] = {}), c
        },
        set: function(a, b, c) {
            var d, e = this.key(a),
                f = this.cache[e];
            if ("string" == typeof b) f[b] = c;
            else if (n.isEmptyObject(f)) n.extend(this.cache[e], b);
            else
                for (d in b) f[d] = b[d];
            return f
        },
        get: function(a, b) {
            var c = this.cache[this.key(a)];
            return void 0 === b ? c : c[b]
        },
        access: function(a, b, c) {
            var d;
            return void 0 === b || b && "string" == typeof b && void 0 === c ? (d = this.get(a, b), void 0 !== d ? d : this.get(a, n.camelCase(b))) : (this.set(a, b, c), void 0 !== c ? c : b)
        },
        remove: function(a, b) {
            var c, d, e, f = this.key(a),
                g = this.cache[f];
            if (void 0 === b) this.cache[f] = {};
            else {
                n.isArray(b) ? d = b.concat(b.map(n.camelCase)) : (e = n.camelCase(b), b in g ? d = [b, e] : (d = e, d = d in g ? [d] : d.match(H) || [])), c = d.length;
                while (c--) delete g[d[c]]
            }
        },
        hasData: function(a) {
            return !n.isEmptyObject(this.cache[a[this.expando]] || {})
        },
        discard: function(a) {
            a[this.expando] && delete this.cache[a[this.expando]]
        }
    };
    var O = new N,
        P = new N,
        Q = /^(?:\{[\w\W]*\}|\[[\w\W]*\])$/,
        R = /([A-Z])/g;

    function S(a, b, c) {
        var d;
        if (void 0 === c && 1 === a.nodeType)
            if (d = "data-" + b.replace(R, "-$1").toLowerCase(), c = a.getAttribute(d), "string" == typeof c) {
                try {
                    c = "true" === c ? !0 : "false" === c ? !1 : "null" === c ? null : +c + "" === c ? +c : Q.test(c) ? n.parseJSON(c) : c
                } catch (e) {}
                P.set(a, b, c)
            } else c = void 0;
        return c
    }
    n.extend({
        hasData: function(a) {
            return P.hasData(a) || O.hasData(a)
        },
        data: function(a, b, c) {
            return P.access(a, b, c)
        },
        removeData: function(a, b) {
            P.remove(a, b)
        },
        _data: function(a, b, c) {
            return O.access(a, b, c)
        },
        _removeData: function(a, b) {
            O.remove(a, b)
        }
    }), n.fn.extend({
        data: function(a, b) {
            var c, d, e, f = this[0],
                g = f && f.attributes;
            if (void 0 === a) {
                if (this.length && (e = P.get(f), 1 === f.nodeType && !O.get(f, "hasDataAttrs"))) {
                    c = g.length;
                    while (c--) g[c] && (d = g[c].name, 0 === d.indexOf("data-") && (d = n.camelCase(d.slice(5)), S(f, d, e[d])));
                    O.set(f, "hasDataAttrs", !0)
                }
                return e
            }
            return "object" == typeof a ? this.each(function() {
                P.set(this, a)
            }) : M(this, function(b) {
                var c, d = n.camelCase(a);
                if (f && void 0 === b) {
                    if (c = P.get(f, a), void 0 !== c) return c;
                    if (c = P.get(f, d), void 0 !== c) return c;
                    if (c = S(f, d, void 0), void 0 !== c) return c
                } else this.each(function() {
                    var c = P.get(this, d);
                    P.set(this, d, b), -1 !== a.indexOf("-") && void 0 !== c && P.set(this, a, b)
                })
            }, null, b, arguments.length > 1, null, !0)
        },
        removeData: function(a) {
            return this.each(function() {
                P.remove(this, a)
            })
        }
    }), n.extend({
        queue: function(a, b, c) {
            var d;
            return a ? (b = (b || "fx") + "queue", d = O.get(a, b), c && (!d || n.isArray(c) ? d = O.access(a, b, n.makeArray(c)) : d.push(c)), d || []) : void 0
        },
        dequeue: function(a, b) {
            b = b || "fx";
            var c = n.queue(a, b),
                d = c.length,
                e = c.shift(),
                f = n._queueHooks(a, b),
                g = function() {
                    n.dequeue(a, b)
                };
            "inprogress" === e && (e = c.shift(), d--), e && ("fx" === b && c.unshift("inprogress"), delete f.stop, e.call(a, g, f)), !d && f && f.empty.fire()
        },
        _queueHooks: function(a, b) {
            var c = b + "queueHooks";
            return O.get(a, c) || O.access(a, c, {
                empty: n.Callbacks("once memory").add(function() {
                    O.remove(a, [b + "queue", c])
                })
            })
        }
    }), n.fn.extend({
        queue: function(a, b) {
            var c = 2;
            return "string" != typeof a && (b = a, a = "fx", c--), arguments.length < c ? n.queue(this[0], a) : void 0 === b ? this : this.each(function() {
                var c = n.queue(this, a, b);
                n._queueHooks(this, a), "fx" === a && "inprogress" !== c[0] && n.dequeue(this, a)
            })
        },
        dequeue: function(a) {
            return this.each(function() {
                n.dequeue(this, a)
            })
        },
        clearQueue: function(a) {
            return this.queue(a || "fx", [])
        },
        promise: function(a, b) {
            var c, d = 1,
                e = n.Deferred(),
                f = this,
                g = this.length,
                h = function() {
                    --d || e.resolveWith(f, [f])
                };
            "string" != typeof a && (b = a, a = void 0), a = a || "fx";
            while (g--) c = O.get(f[g], a + "queueHooks"), c && c.empty && (d++, c.empty.add(h));
            return h(), e.promise(b)
        }
    });
    var T = /[+-]?(?:\d*\.|)\d+(?:[eE][+-]?\d+|)/.source,
        U = /^(?:checkbox|radio)$/i;
    ! function() {
        var a = l.createDocumentFragment(),
            b = a.appendChild(l.createElement("div")),
            c = l.createElement("input");
        c.setAttribute("type", "radio"), c.setAttribute("checked", "checked"), c.setAttribute("name", "t"), b.appendChild(c), k.checkClone = b.cloneNode(!0).cloneNode(!0).lastChild.checked, b.innerHTML = "<textarea>x</textarea>", k.noCloneChecked = !!b.cloneNode(!0).lastChild.defaultValue
    }();
    var V = "undefined";
    k.focusinBubbles = "onfocusin" in a;
    var W = /^key/,
        X = /^(?:mouse|pointer|contextmenu)|click/,
        Y = /^(?:focusinfocus|focusoutblur)$/,
        Z = /^([^.]*)(?:\.(.+)|)$/;

    function $() {
        return !0
    }

    function _() {
        return !1
    }

    function ab() {
        try {
            return l.activeElement
        } catch (a) {}
    }
    n.event = {
        global: {},
        add: function(a, b, c, d, e) {
            var f, g, h, i, j, k, l, m, o, p, q, r = O.get(a);
            if (r) {
                c.handler && (f = c, c = f.handler, e = f.selector), c.guid || (c.guid = n.guid++), (i = r.events) || (i = r.events = {}), (g = r.handle) || (g = r.handle = function(b) {
                    return typeof n !== V && n.event.triggered !== b.type ? n.event.dispatch.apply(a, arguments) : void 0
                }), b = (b || "").match(H) || [""], j = b.length;
                while (j--) h = Z.exec(b[j]) || [], o = q = h[1], p = (h[2] || "").split(".").sort(), o && (l = n.event.special[o] || {}, o = (e ? l.delegateType : l.bindType) || o, l = n.event.special[o] || {}, k = n.extend({
                    type: o,
                    origType: q,
                    data: d,
                    handler: c,
                    guid: c.guid,
                    selector: e,
                    needsContext: e && n.expr.match.needsContext.test(e),
                    namespace: p.join(".")
                }, f), (m = i[o]) || (m = i[o] = [], m.delegateCount = 0, l.setup && l.setup.call(a, d, p, g) !== !1 || a.addEventListener && a.addEventListener(o, g, !1)), l.add && (l.add.call(a, k), k.handler.guid || (k.handler.guid = c.guid)), e ? m.splice(m.delegateCount++, 0, k) : m.push(k), n.event.global[o] = !0)
            }
        },
        remove: function(a, b, c, d, e) {
            var f, g, h, i, j, k, l, m, o, p, q, r = O.hasData(a) && O.get(a);
            if (r && (i = r.events)) {
                b = (b || "").match(H) || [""], j = b.length;
                while (j--)
                    if (h = Z.exec(b[j]) || [], o = q = h[1], p = (h[2] || "").split(".").sort(), o) {
                        l = n.event.special[o] || {}, o = (d ? l.delegateType : l.bindType) || o, m = i[o] || [], h = h[2] && new RegExp("(^|\\.)" + p.join("\\.(?:.*\\.|)") + "(\\.|$)"), g = f = m.length;
                        while (f--) k = m[f], !e && q !== k.origType || c && c.guid !== k.guid || h && !h.test(k.namespace) || d && d !== k.selector && ("**" !== d || !k.selector) || (m.splice(f, 1), k.selector && m.delegateCount--, l.remove && l.remove.call(a, k));
                        g && !m.length && (l.teardown && l.teardown.call(a, p, r.handle) !== !1 || n.removeEvent(a, o, r.handle), delete i[o])
                    } else
                        for (o in i) n.event.remove(a, o + b[j], c, d, !0);
                n.isEmptyObject(i) && (delete r.handle, O.remove(a, "events"))
            }
        },
        trigger: function(b, c, d, e) {
            var f, g, h, i, k, m, o, p = [d || l],
                q = j.call(b, "type") ? b.type : b,
                r = j.call(b, "namespace") ? b.namespace.split(".") : [];
            if (g = h = d = d || l, 3 !== d.nodeType && 8 !== d.nodeType && !Y.test(q + n.event.triggered) && (q.indexOf(".") >= 0 && (r = q.split("."), q = r.shift(), r.sort()), k = q.indexOf(":") < 0 && "on" + q, b = b[n.expando] ? b : new n.Event(q, "object" == typeof b && b), b.isTrigger = e ? 2 : 3, b.namespace = r.join("."), b.namespace_re = b.namespace ? new RegExp("(^|\\.)" + r.join("\\.(?:.*\\.|)") + "(\\.|$)") : null, b.result = void 0, b.target || (b.target = d), c = null == c ? [b] : n.makeArray(c, [b]), o = n.event.special[q] || {}, e || !o.trigger || o.trigger.apply(d, c) !== !1)) {
                if (!e && !o.noBubble && !n.isWindow(d)) {
                    for (i = o.delegateType || q, Y.test(i + q) || (g = g.parentNode); g; g = g.parentNode) p.push(g), h = g;
                    h === (d.ownerDocument || l) && p.push(h.defaultView || h.parentWindow || a)
                }
                f = 0;
                while ((g = p[f++]) && !b.isPropagationStopped()) b.type = f > 1 ? i : o.bindType || q, m = (O.get(g, "events") || {})[b.type] && O.get(g, "handle"), m && m.apply(g, c), m = k && g[k], m && m.apply && n.acceptData(g) && (b.result = m.apply(g, c), b.result === !1 && b.preventDefault());
                return b.type = q, e || b.isDefaultPrevented() || o._default && o._default.apply(p.pop(), c) !== !1 || !n.acceptData(d) || k && n.isFunction(d[q]) && !n.isWindow(d) && (h = d[k], h && (d[k] = null), n.event.triggered = q, d[q](), n.event.triggered = void 0, h && (d[k] = h)), b.result
            }
        },
        dispatch: function(a) {
            a = n.event.fix(a);
            var b, c, e, f, g, h = [],
                i = d.call(arguments),
                j = (O.get(this, "events") || {})[a.type] || [],
                k = n.event.special[a.type] || {};
            if (i[0] = a, a.delegateTarget = this, !k.preDispatch || k.preDispatch.call(this, a) !== !1) {
                h = n.event.handlers.call(this, a, j), b = 0;
                while ((f = h[b++]) && !a.isPropagationStopped()) {
                    a.currentTarget = f.elem, c = 0;
                    while ((g = f.handlers[c++]) && !a.isImmediatePropagationStopped())(!a.namespace_re || a.namespace_re.test(g.namespace)) && (a.handleObj = g, a.data = g.data, e = ((n.event.special[g.origType] || {}).handle || g.handler).apply(f.elem, i), void 0 !== e && (a.result = e) === !1 && (a.preventDefault(), a.stopPropagation()))
                }
                return k.postDispatch && k.postDispatch.call(this, a), a.result
            }
        },
        handlers: function(a, b) {
            var c, d, e, f, g = [],
                h = b.delegateCount,
                i = a.target;
            if (h && i.nodeType && (!a.button || "click" !== a.type))
                for (; i !== this; i = i.parentNode || this)
                    if (i.disabled !== !0 || "click" !== a.type) {
                        for (d = [], c = 0; h > c; c++) f = b[c], e = f.selector + " ", void 0 === d[e] && (d[e] = f.needsContext ? n(e, this).index(i) >= 0 : n.find(e, this, null, [i]).length), d[e] && d.push(f);
                        d.length && g.push({
                            elem: i,
                            handlers: d
                        })
                    } return h < b.length && g.push({
                elem: this,
                handlers: b.slice(h)
            }), g
        },
        props: "altKey bubbles cancelable ctrlKey currentTarget eventPhase metaKey relatedTarget shiftKey target timeStamp view which".split(" "),
        fixHooks: {},
        keyHooks: {
            props: "char charCode key keyCode".split(" "),
            filter: function(a, b) {
                return null == a.which && (a.which = null != b.charCode ? b.charCode : b.keyCode), a
            }
        },
        mouseHooks: {
            props: "button buttons clientX clientY offsetX offsetY pageX pageY screenX screenY toElement".split(" "),
            filter: function(a, b) {
                var c, d, e, f = b.button;
                return null == a.pageX && null != b.clientX && (c = a.target.ownerDocument || l, d = c.documentElement, e = c.body, a.pageX = b.clientX + (d && d.scrollLeft || e && e.scrollLeft || 0) - (d && d.clientLeft || e && e.clientLeft || 0), a.pageY = b.clientY + (d && d.scrollTop || e && e.scrollTop || 0) - (d && d.clientTop || e && e.clientTop || 0)), a.which || void 0 === f || (a.which = 1 & f ? 1 : 2 & f ? 3 : 4 & f ? 2 : 0), a
            }
        },
        fix: function(a) {
            if (a[n.expando]) return a;
            var b, c, d, e = a.type,
                f = a,
                g = this.fixHooks[e];
            g || (this.fixHooks[e] = g = X.test(e) ? this.mouseHooks : W.test(e) ? this.keyHooks : {}), d = g.props ? this.props.concat(g.props) : this.props, a = new n.Event(f), b = d.length;
            while (b--) c = d[b], a[c] = f[c];
            return a.target || (a.target = l), 3 === a.target.nodeType && (a.target = a.target.parentNode), g.filter ? g.filter(a, f) : a
        },
        special: {
            load: {
                noBubble: !0
            },
            focus: {
                trigger: function() {
                    return this !== ab() && this.focus ? (this.focus(), !1) : void 0
                },
                delegateType: "focusin"
            },
            blur: {
                trigger: function() {
                    return this === ab() && this.blur ? (this.blur(), !1) : void 0
                },
                delegateType: "focusout"
            },
            click: {
                trigger: function() {
                    return "checkbox" === this.type && this.click && n.nodeName(this, "input") ? (this.click(), !1) : void 0
                },
                _default: function(a) {
                    return n.nodeName(a.target, "a")
                }
            },
            beforeunload: {
                postDispatch: function(a) {
                    void 0 !== a.result && a.originalEvent && (a.originalEvent.returnValue = a.result)
                }
            }
        },
        simulate: function(a, b, c, d) {
            var e = n.extend(new n.Event, c, {
                type: a,
                isSimulated: !0,
                originalEvent: {}
            });
            d ? n.event.trigger(e, null, b) : n.event.dispatch.call(b, e), e.isDefaultPrevented() && c.preventDefault()
        }
    }, n.removeEvent = function(a, b, c) {
        a.removeEventListener && a.removeEventListener(b, c, !1)
    }, n.Event = function(a, b) {
        return this instanceof n.Event ? (a && a.type ? (this.originalEvent = a, this.type = a.type, this.isDefaultPrevented = a.defaultPrevented || void 0 === a.defaultPrevented && a.returnValue === !1 ? $ : _) : this.type = a, b && n.extend(this, b), this.timeStamp = a && a.timeStamp || n.now(), void(this[n.expando] = !0)) : new n.Event(a, b)
    }, n.Event.prototype = {
        isDefaultPrevented: _,
        isPropagationStopped: _,
        isImmediatePropagationStopped: _,
        preventDefault: function() {
            var a = this.originalEvent;
            this.isDefaultPrevented = $, a && a.preventDefault && a.preventDefault()
        },
        stopPropagation: function() {
            var a = this.originalEvent;
            this.isPropagationStopped = $, a && a.stopPropagation && a.stopPropagation()
        },
        stopImmediatePropagation: function() {
            var a = this.originalEvent;
            this.isImmediatePropagationStopped = $, a && a.stopImmediatePropagation && a.stopImmediatePropagation(), this.stopPropagation()
        }
    }, n.each({
        mouseenter: "mouseover",
        mouseleave: "mouseout",
        pointerenter: "pointerover",
        pointerleave: "pointerout"
    }, function(a, b) {
        n.event.special[a] = {
            delegateType: b,
            bindType: b,
            handle: function(a) {
                var c, d = this,
                    e = a.relatedTarget,
                    f = a.handleObj;
                return (!e || e !== d && !n.contains(d, e)) && (a.type = f.origType, c = f.handler.apply(this, arguments), a.type = b), c
            }
        }
    }), k.focusinBubbles || n.each({
        focus: "focusin",
        blur: "focusout"
    }, function(a, b) {
        var c = function(a) {
            n.event.simulate(b, a.target, n.event.fix(a), !0)
        };
        n.event.special[b] = {
            setup: function() {
                var d = this.ownerDocument || this,
                    e = O.access(d, b);
                e || d.addEventListener(a, c, !0), O.access(d, b, (e || 0) + 1)
            },
            teardown: function() {
                var d = this.ownerDocument || this,
                    e = O.access(d, b) - 1;
                e ? O.access(d, b, e) : (d.removeEventListener(a, c, !0), O.remove(d, b))
            }
        }
    }), n.fn.extend({
        on: function(a, b, c, d, e) {
            var f, g;
            if ("object" == typeof a) {
                "string" != typeof b && (c = c || b, b = void 0);
                for (g in a) this.on(g, b, c, a[g], e);
                return this
            }
            if (null == c && null == d ? (d = b, c = b = void 0) : null == d && ("string" == typeof b ? (d = c, c = void 0) : (d = c, c = b, b = void 0)), d === !1) d = _;
            else if (!d) return this;
            return 1 === e && (f = d, d = function(a) {
                return n().off(a), f.apply(this, arguments)
            }, d.guid = f.guid || (f.guid = n.guid++)), this.each(function() {
                n.event.add(this, a, d, c, b)
            })
        },
        one: function(a, b, c, d) {
            return this.on(a, b, c, d, 1)
        },
        off: function(a, b, c) {
            var d, e;
            if (a && a.preventDefault && a.handleObj) return d = a.handleObj, n(a.delegateTarget).off(d.namespace ? d.origType + "." + d.namespace : d.origType, d.selector, d.handler), this;
            if ("object" == typeof a) {
                for (e in a) this.off(e, b, a[e]);
                return this
            }
            return (b === !1 || "function" == typeof b) && (c = b, b = void 0), c === !1 && (c = _), this.each(function() {
                n.event.remove(this, a, c, b)
            })
        },
        trigger: function(a, b) {
            return this.each(function() {
                n.event.trigger(a, b, this)
            })
        },
        triggerHandler: function(a, b) {
            var c = this[0];
            return c ? n.event.trigger(a, b, c, !0) : void 0
        }
    });
    var bb = /<(?!area|br|col|embed|hr|img|input|link|meta|param)(([\w:]+)[^>]*)\/>/gi,
        cb = /<([\w:]+)/,
        db = /<|&#?\w+;/,
        eb = /<(?:script|style|link)/i,
        fb = /checked\s*(?:[^=]|=\s*.checked.)/i,
        gb = /^$|\/(?:java|ecma)script/i,
        hb = /^true\/(.*)/,
        ib = /^\s*<!(?:\[CDATA\[|--)|(?:\]\]|--)>\s*$/g,
        jb = {
            option: [1, "<select multiple='multiple'>", "</select>"],
            thead: [1, "<table>", "</table>"],
            col: [2, "<table><colgroup>", "</colgroup></table>"],
            tr: [2, "<table><tbody>", "</tbody></table>"],
            td: [3, "<table><tbody><tr>", "</tr></tbody></table>"],
            _default: [0, "", ""]
        };
    jb.optgroup = jb.option, jb.tbody = jb.tfoot = jb.colgroup = jb.caption = jb.thead, jb.th = jb.td;

    function kb(a, b) {
        return n.nodeName(a, "table") && n.nodeName(11 !== b.nodeType ? b : b.firstChild, "tr") ? a.getElementsByTagName("tbody")[0] || a.appendChild(a.ownerDocument.createElement("tbody")) : a
    }

    function lb(a) {
        return a.type = (null !== a.getAttribute("type")) + "/" + a.type, a
    }

    function mb(a) {
        var b = hb.exec(a.type);
        return b ? a.type = b[1] : a.removeAttribute("type"), a
    }

    function nb(a, b) {
        for (var c = 0, d = a.length; d > c; c++) O.set(a[c], "globalEval", !b || O.get(b[c], "globalEval"))
    }

    function ob(a, b) {
        var c, d, e, f, g, h, i, j;
        if (1 === b.nodeType) {
            if (O.hasData(a) && (f = O.access(a), g = O.set(b, f), j = f.events)) {
                delete g.handle, g.events = {};
                for (e in j)
                    for (c = 0, d = j[e].length; d > c; c++) n.event.add(b, e, j[e][c])
            }
            P.hasData(a) && (h = P.access(a), i = n.extend({}, h), P.set(b, i))
        }
    }

    function pb(a, b) {
        var c = a.getElementsByTagName ? a.getElementsByTagName(b || "*") : a.querySelectorAll ? a.querySelectorAll(b || "*") : [];
        return void 0 === b || b && n.nodeName(a, b) ? n.merge([a], c) : c
    }

    function qb(a, b) {
        var c = b.nodeName.toLowerCase();
        "input" === c && U.test(a.type) ? b.checked = a.checked : ("input" === c || "textarea" === c) && (b.defaultValue = a.defaultValue)
    }
    n.extend({
            clone: function(a, b, c) {
                var d, e, f, g, h = a.cloneNode(!0),
                    i = n.contains(a.ownerDocument, a);
                if (!(k.noCloneChecked || 1 !== a.nodeType && 11 !== a.nodeType || n.isXMLDoc(a)))
                    for (g = pb(h), f = pb(a), d = 0, e = f.length; e > d; d++) qb(f[d], g[d]);
                if (b)
                    if (c)
                        for (f = f || pb(a), g = g || pb(h), d = 0, e = f.length; e > d; d++) ob(f[d], g[d]);
                    else ob(a, h);
                return g = pb(h, "script"), g.length > 0 && nb(g, !i && pb(a, "script")), h
            },
            buildFragment: function(a, b, c, d) {
                for (var e, f, g, h, i, j, k = b.createDocumentFragment(), l = [], m = 0, o = a.length; o > m; m++)
                    if (e = a[m], e || 0 === e)
                        if ("object" === n.type(e)) n.merge(l, e.nodeType ? [e] : e);
                        else if (db.test(e)) {
                    f = f || k.appendChild(b.createElement("div")), g = (cb.exec(e) || ["", ""])[1].toLowerCase(), h = jb[g] || jb._default, f.innerHTML = h[1] + e.replace(bb, "<$1></$2>") + h[2], j = h[0];
                    while (j--) f = f.lastChild;
                    n.merge(l, f.childNodes), f = k.firstChild, f.textContent = ""
                } else l.push(b.createTextNode(e));
                k.textContent = "", m = 0;
                while (e = l[m++])
                    if ((!d || -1 === n.inArray(e, d)) && (i = n.contains(e.ownerDocument, e), f = pb(k.appendChild(e), "script"), i && nb(f), c)) {
                        j = 0;
                        while (e = f[j++]) gb.test(e.type || "") && c.push(e)
                    } return k
            },
            cleanData: function(a) {
                for (var b, c, d, e, f = n.event.special, g = 0; void 0 !== (c = a[g]); g++) {
                    if (n.acceptData(c) && (e = c[O.expando], e && (b = O.cache[e]))) {
                        if (b.events)
                            for (d in b.events) f[d] ? n.event.remove(c, d) : n.removeEvent(c, d, b.handle);
                        O.cache[e] && delete O.cache[e]
                    }
                    delete P.cache[c[P.expando]]
                }
            }
        }), n.fn.extend({
            text: function(a) {
                return M(this, function(a) {
                    return void 0 === a ? n.text(this) : this.empty().each(function() {
                        (1 === this.nodeType || 11 === this.nodeType || 9 === this.nodeType) && (this.textContent = a)
                    })
                }, null, a, arguments.length)
            },
            append: function() {
                return this.domManip(arguments, function(a) {
                    if (1 === this.nodeType || 11 === this.nodeType || 9 === this.nodeType) {
                        var b = kb(this, a);
                        b.appendChild(a)
                    }
                })
            },
            prepend: function() {
                return this.domManip(arguments, function(a) {
                    if (1 === this.nodeType || 11 === this.nodeType || 9 === this.nodeType) {
                        var b = kb(this, a);
                        b.insertBefore(a, b.firstChild)
                    }
                })
            },
            before: function() {
                return this.domManip(arguments, function(a) {
                    this.parentNode && this.parentNode.insertBefore(a, this)
                })
            },
            after: function() {
                return this.domManip(arguments, function(a) {
                    this.parentNode && this.parentNode.insertBefore(a, this.nextSibling)
                })
            },
            remove: function(a, b) {
                for (var c, d = a ? n.filter(a, this) : this, e = 0; null != (c = d[e]); e++) b || 1 !== c.nodeType || n.cleanData(pb(c)), c.parentNode && (b && n.contains(c.ownerDocument, c) && nb(pb(c, "script")), c.parentNode.removeChild(c));
                return this
            },
            empty: function() {
                for (var a, b = 0; null != (a = this[b]); b++) 1 === a.nodeType && (n.cleanData(pb(a, !1)), a.textContent = "");
                return this
            },
            clone: function(a, b) {
                return a = null == a ? !1 : a, b = null == b ? a : b, this.map(function() {
                    return n.clone(this, a, b)
                })
            },
            html: function(a) {
                return M(this, function(a) {
                    var b = this[0] || {},
                        c = 0,
                        d = this.length;
                    if (void 0 === a && 1 === b.nodeType) return b.innerHTML;
                    if ("string" == typeof a && !eb.test(a) && !jb[(cb.exec(a) || ["", ""])[1].toLowerCase()]) {
                        a = a.replace(bb, "<$1></$2>");
                        try {
                            for (; d > c; c++) b = this[c] || {}, 1 === b.nodeType && (n.cleanData(pb(b, !1)), b.innerHTML = a);
                            b = 0
                        } catch (e) {}
                    }
                    b && this.empty().append(a)
                }, null, a, arguments.length)
            },
            replaceWith: function() {
                var a = arguments[0];
                return this.domManip(arguments, function(b) {
                    a = this.parentNode, n.cleanData(pb(this)), a && a.replaceChild(b, this)
                }), a && (a.length || a.nodeType) ? this : this.remove()
            },
            detach: function(a) {
                return this.remove(a, !0)
            },
            domManip: function(a, b) {
                a = e.apply([], a);
                var c, d, f, g, h, i, j = 0,
                    l = this.length,
                    m = this,
                    o = l - 1,
                    p = a[0],
                    q = n.isFunction(p);
                if (q || l > 1 && "string" == typeof p && !k.checkClone && fb.test(p)) return this.each(function(c) {
                    var d = m.eq(c);
                    q && (a[0] = p.call(this, c, d.html())), d.domManip(a, b)
                });
                if (l && (c = n.buildFragment(a, this[0].ownerDocument, !1, this), d = c.firstChild, 1 === c.childNodes.length && (c = d), d)) {
                    for (f = n.map(pb(c, "script"), lb), g = f.length; l > j; j++) h = c, j !== o && (h = n.clone(h, !0, !0), g && n.merge(f, pb(h, "script"))), b.call(this[j], h, j);
                    if (g)
                        for (i = f[f.length - 1].ownerDocument, n.map(f, mb), j = 0; g > j; j++) h = f[j], gb.test(h.type || "") && !O.access(h, "globalEval") && n.contains(i, h) && (h.src ? n._evalUrl && n._evalUrl(h.src) : n.globalEval(h.textContent.replace(ib, "")))
                }
                return this
            }
        }), n.each({
            appendTo: "append",
            prependTo: "prepend",
            insertBefore: "before",
            insertAfter: "after",
            replaceAll: "replaceWith"
        }, function(a, b) {
            n.fn[a] = function(a) {
                for (var c, d = [], e = n(a), g = e.length - 1, h = 0; g >= h; h++) c = h === g ? this : this.clone(!0), n(e[h])[b](c), f.apply(d, c.get());
                return this.pushStack(d)
            }
        }), n.fn.delay = function(a, b) {
            return a = n.fx ? n.fx.speeds[a] || a : a, b = b || "fx", this.queue(b, function(b, c) {
                var d = setTimeout(b, a);
                c.stop = function() {
                    clearTimeout(d)
                }
            })
        },
        function() {
            var a = l.createElement("input"),
                b = l.createElement("select"),
                c = b.appendChild(l.createElement("option"));
            a.type = "checkbox", k.checkOn = "" !== a.value, k.optSelected = c.selected, b.disabled = !0, k.optDisabled = !c.disabled, a = l.createElement("input"), a.value = "t", a.type = "radio", k.radioValue = "t" === a.value
        }();
    var rb, sb, tb = n.expr.attrHandle;
    n.fn.extend({
        attr: function(a, b) {
            return M(this, n.attr, a, b, arguments.length > 1)
        },
        removeAttr: function(a) {
            return this.each(function() {
                n.removeAttr(this, a)
            })
        }
    }), n.extend({
        attr: function(a, b, c) {
            var d, e, f = a.nodeType;
            if (a && 3 !== f && 8 !== f && 2 !== f) return typeof a.getAttribute === V ? n.prop(a, b, c) : (1 === f && n.isXMLDoc(a) || (b = b.toLowerCase(), d = n.attrHooks[b] || (n.expr.match.bool.test(b) ? sb : rb)), void 0 === c ? d && "get" in d && null !== (e = d.get(a, b)) ? e : (e = n.find.attr(a, b), null == e ? void 0 : e) : null !== c ? d && "set" in d && void 0 !== (e = d.set(a, c, b)) ? e : (a.setAttribute(b, c + ""), c) : void n.removeAttr(a, b))
        },
        removeAttr: function(a, b) {
            var c, d, e = 0,
                f = b && b.match(H);
            if (f && 1 === a.nodeType)
                while (c = f[e++]) d = n.propFix[c] || c, n.expr.match.bool.test(c) && (a[d] = !1), a.removeAttribute(c)
        },
        attrHooks: {
            type: {
                set: function(a, b) {
                    if (!k.radioValue && "radio" === b && n.nodeName(a, "input")) {
                        var c = a.value;
                        return a.setAttribute("type", b), c && (a.value = c), b
                    }
                }
            }
        }
    }), sb = {
        set: function(a, b, c) {
            return b === !1 ? n.removeAttr(a, c) : a.setAttribute(c, c), c
        }
    }, n.each(n.expr.match.bool.source.match(/\w+/g), function(a, b) {
        var c = tb[b] || n.find.attr;
        tb[b] = function(a, b, d) {
            var e, f;
            return d || (f = tb[b], tb[b] = e, e = null != c(a, b, d) ? b.toLowerCase() : null, tb[b] = f), e
        }
    });
    var ub = /^(?:input|select|textarea|button)$/i;
    n.fn.extend({
        prop: function(a, b) {
            return M(this, n.prop, a, b, arguments.length > 1)
        },
        removeProp: function(a) {
            return this.each(function() {
                delete this[n.propFix[a] || a]
            })
        }
    }), n.extend({
        propFix: {
            "for": "htmlFor",
            "class": "className"
        },
        prop: function(a, b, c) {
            var d, e, f, g = a.nodeType;
            if (a && 3 !== g && 8 !== g && 2 !== g) return f = 1 !== g || !n.isXMLDoc(a), f && (b = n.propFix[b] || b, e = n.propHooks[b]), void 0 !== c ? e && "set" in e && void 0 !== (d = e.set(a, c, b)) ? d : a[b] = c : e && "get" in e && null !== (d = e.get(a, b)) ? d : a[b]
        },
        propHooks: {
            tabIndex: {
                get: function(a) {
                    return a.hasAttribute("tabindex") || ub.test(a.nodeName) || a.href ? a.tabIndex : -1
                }
            }
        }
    }), k.optSelected || (n.propHooks.selected = {
        get: function(a) {
            var b = a.parentNode;
            return b && b.parentNode && b.parentNode.selectedIndex, null
        }
    }), n.each(["tabIndex", "readOnly", "maxLength", "cellSpacing", "cellPadding", "rowSpan", "colSpan", "useMap", "frameBorder", "contentEditable"], function() {
        n.propFix[this.toLowerCase()] = this
    });
    var vb = /[\t\r\n\f]/g;
    n.fn.extend({
        addClass: function(a) {
            var b, c, d, e, f, g, h = "string" == typeof a && a,
                i = 0,
                j = this.length;
            if (n.isFunction(a)) return this.each(function(b) {
                n(this).addClass(a.call(this, b, this.className))
            });
            if (h)
                for (b = (a || "").match(H) || []; j > i; i++)
                    if (c = this[i], d = 1 === c.nodeType && (c.className ? (" " + c.className + " ").replace(vb, " ") : " ")) {
                        f = 0;
                        while (e = b[f++]) d.indexOf(" " + e + " ") < 0 && (d += e + " ");
                        g = n.trim(d), c.className !== g && (c.className = g)
                    } return this
        },
        removeClass: function(a) {
            var b, c, d, e, f, g, h = 0 === arguments.length || "string" == typeof a && a,
                i = 0,
                j = this.length;
            if (n.isFunction(a)) return this.each(function(b) {
                n(this).removeClass(a.call(this, b, this.className))
            });
            if (h)
                for (b = (a || "").match(H) || []; j > i; i++)
                    if (c = this[i], d = 1 === c.nodeType && (c.className ? (" " + c.className + " ").replace(vb, " ") : "")) {
                        f = 0;
                        while (e = b[f++])
                            while (d.indexOf(" " + e + " ") >= 0) d = d.replace(" " + e + " ", " ");
                        g = a ? n.trim(d) : "", c.className !== g && (c.className = g)
                    } return this
        },
        toggleClass: function(a, b) {
            var c = typeof a;
            return "boolean" == typeof b && "string" === c ? b ? this.addClass(a) : this.removeClass(a) : this.each(n.isFunction(a) ? function(c) {
                n(this).toggleClass(a.call(this, c, this.className, b), b)
            } : function() {
                if ("string" === c) {
                    var b, d = 0,
                        e = n(this),
                        f = a.match(H) || [];
                    while (b = f[d++]) e.hasClass(b) ? e.removeClass(b) : e.addClass(b)
                } else(c === V || "boolean" === c) && (this.className && O.set(this, "__className__", this.className), this.className = this.className || a === !1 ? "" : O.get(this, "__className__") || "")
            })
        },
        hasClass: function(a) {
            for (var b = " " + a + " ", c = 0, d = this.length; d > c; c++)
                if (1 === this[c].nodeType && (" " + this[c].className + " ").replace(vb, " ").indexOf(b) >= 0) return !0;
            return !1
        }
    });
    var wb = /\r/g;
    n.fn.extend({
        val: function(a) {
            var b, c, d, e = this[0]; {
                if (arguments.length) return d = n.isFunction(a), this.each(function(c) {
                    var e;
                    1 === this.nodeType && (e = d ? a.call(this, c, n(this).val()) : a, null == e ? e = "" : "number" == typeof e ? e += "" : n.isArray(e) && (e = n.map(e, function(a) {
                        return null == a ? "" : a + ""
                    })), b = n.valHooks[this.type] || n.valHooks[this.nodeName.toLowerCase()], b && "set" in b && void 0 !== b.set(this, e, "value") || (this.value = e))
                });
                if (e) return b = n.valHooks[e.type] || n.valHooks[e.nodeName.toLowerCase()], b && "get" in b && void 0 !== (c = b.get(e, "value")) ? c : (c = e.value, "string" == typeof c ? c.replace(wb, "") : null == c ? "" : c)
            }
        }
    }), n.extend({
        valHooks: {
            option: {
                get: function(a) {
                    var b = n.find.attr(a, "value");
                    return null != b ? b : n.trim(n.text(a))
                }
            },
            select: {
                get: function(a) {
                    for (var b, c, d = a.options, e = a.selectedIndex, f = "select-one" === a.type || 0 > e, g = f ? null : [], h = f ? e + 1 : d.length, i = 0 > e ? h : f ? e : 0; h > i; i++)
                        if (c = d[i], !(!c.selected && i !== e || (k.optDisabled ? c.disabled : null !== c.getAttribute("disabled")) || c.parentNode.disabled && n.nodeName(c.parentNode, "optgroup"))) {
                            if (b = n(c).val(), f) return b;
                            g.push(b)
                        } return g
                },
                set: function(a, b) {
                    var c, d, e = a.options,
                        f = n.makeArray(b),
                        g = e.length;
                    while (g--) d = e[g], (d.selected = n.inArray(d.value, f) >= 0) && (c = !0);
                    return c || (a.selectedIndex = -1), f
                }
            }
        }
    }), n.each(["radio", "checkbox"], function() {
        n.valHooks[this] = {
            set: function(a, b) {
                return n.isArray(b) ? a.checked = n.inArray(n(a).val(), b) >= 0 : void 0
            }
        }, k.checkOn || (n.valHooks[this].get = function(a) {
            return null === a.getAttribute("value") ? "on" : a.value
        })
    }), n.each("blur focus focusin focusout load resize scroll unload click dblclick mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave change select submit keydown keypress keyup error contextmenu".split(" "), function(a, b) {
        n.fn[b] = function(a, c) {
            return arguments.length > 0 ? this.on(b, null, a, c) : this.trigger(b)
        }
    }), n.fn.extend({
        hover: function(a, b) {
            return this.mouseenter(a).mouseleave(b || a)
        },
        bind: function(a, b, c) {
            return this.on(a, null, b, c)
        },
        unbind: function(a, b) {
            return this.off(a, null, b)
        },
        delegate: function(a, b, c, d) {
            return this.on(b, a, c, d)
        },
        undelegate: function(a, b, c) {
            return 1 === arguments.length ? this.off(a, "**") : this.off(b, a || "**", c)
        }
    });
    var xb = n.now(),
        yb = /\?/;
    n.parseJSON = function(a) {
        return JSON.parse(a + "")
    }, n.parseXML = function(a) {
        var b, c;
        if (!a || "string" != typeof a) return null;
        try {
            c = new DOMParser, b = c.parseFromString(a, "text/xml")
        } catch (d) {
            b = void 0
        }
        return (!b || b.getElementsByTagName("parsererror").length) && n.error("Invalid XML: " + a), b
    };
    var zb, Ab, Bb = /#.*$/,
        Cb = /([?&])_=[^&]*/,
        Db = /^(.*?):[ \t]*([^\r\n]*)$/gm,
        Eb = /^(?:about|app|app-storage|.+-extension|file|res|widget):$/,
        Fb = /^(?:GET|HEAD)$/,
        Gb = /^\/\//,
        Hb = /^([\w.+-]+:)(?:\/\/(?:[^\/?#]*@|)([^\/?#:]*)(?::(\d+)|)|)/,
        Ib = {},
        Jb = {},
        Kb = "*/".concat("*");
    try {
        Ab = location.href
    } catch (Lb) {
        Ab = l.createElement("a"), Ab.href = "", Ab = Ab.href
    }
    zb = Hb.exec(Ab.toLowerCase()) || [];

    function Mb(a) {
        return function(b, c) {
            "string" != typeof b && (c = b, b = "*");
            var d, e = 0,
                f = b.toLowerCase().match(H) || [];
            if (n.isFunction(c))
                while (d = f[e++]) "+" === d[0] ? (d = d.slice(1) || "*", (a[d] = a[d] || []).unshift(c)) : (a[d] = a[d] || []).push(c)
        }
    }

    function Nb(a, b, c, d) {
        var e = {},
            f = a === Jb;

        function g(h) {
            var i;
            return e[h] = !0, n.each(a[h] || [], function(a, h) {
                var j = h(b, c, d);
                return "string" != typeof j || f || e[j] ? f ? !(i = j) : void 0 : (b.dataTypes.unshift(j), g(j), !1)
            }), i
        }
        return g(b.dataTypes[0]) || !e["*"] && g("*")
    }

    function Ob(a, b) {
        var c, d, e = n.ajaxSettings.flatOptions || {};
        for (c in b) void 0 !== b[c] && ((e[c] ? a : d || (d = {}))[c] = b[c]);
        return d && n.extend(!0, a, d), a
    }

    function Pb(a, b, c) {
        var d, e, f, g, h = a.contents,
            i = a.dataTypes;
        while ("*" === i[0]) i.shift(), void 0 === d && (d = a.mimeType || b.getResponseHeader("Content-Type"));
        if (d)
            for (e in h)
                if (h[e] && h[e].test(d)) {
                    i.unshift(e);
                    break
                } if (i[0] in c) f = i[0];
        else {
            for (e in c) {
                if (!i[0] || a.converters[e + " " + i[0]]) {
                    f = e;
                    break
                }
                g || (g = e)
            }
            f = f || g
        }
        return f ? (f !== i[0] && i.unshift(f), c[f]) : void 0
    }

    function Qb(a, b, c, d) {
        var e, f, g, h, i, j = {},
            k = a.dataTypes.slice();
        if (k[1])
            for (g in a.converters) j[g.toLowerCase()] = a.converters[g];
        f = k.shift();
        while (f)
            if (a.responseFields[f] && (c[a.responseFields[f]] = b), !i && d && a.dataFilter && (b = a.dataFilter(b, a.dataType)), i = f, f = k.shift())
                if ("*" === f) f = i;
                else if ("*" !== i && i !== f) {
            if (g = j[i + " " + f] || j["* " + f], !g)
                for (e in j)
                    if (h = e.split(" "), h[1] === f && (g = j[i + " " + h[0]] || j["* " + h[0]])) {
                        g === !0 ? g = j[e] : j[e] !== !0 && (f = h[0], k.unshift(h[1]));
                        break
                    } if (g !== !0)
                if (g && a["throws"]) b = g(b);
                else try {
                    b = g(b)
                } catch (l) {
                    return {
                        state: "parsererror",
                        error: g ? l : "No conversion from " + i + " to " + f
                    }
                }
        }
        return {
            state: "success",
            data: b
        }
    }
    n.extend({
        active: 0,
        lastModified: {},
        etag: {},
        ajaxSettings: {
            url: Ab,
            type: "GET",
            isLocal: Eb.test(zb[1]),
            global: !0,
            processData: !0,
            async: !0,
            contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            accepts: {
                "*": Kb,
                text: "text/plain",
                html: "text/html",
                xml: "application/xml, text/xml",
                json: "application/json, text/javascript"
            },
            contents: {
                xml: /xml/,
                html: /html/,
                json: /json/
            },
            responseFields: {
                xml: "responseXML",
                text: "responseText",
                json: "responseJSON"
            },
            converters: {
                "* text": String,
                "text html": !0,
                "text json": n.parseJSON,
                "text xml": n.parseXML
            },
            flatOptions: {
                url: !0,
                context: !0
            }
        },
        ajaxSetup: function(a, b) {
            return b ? Ob(Ob(a, n.ajaxSettings), b) : Ob(n.ajaxSettings, a)
        },
        ajaxPrefilter: Mb(Ib),
        ajaxTransport: Mb(Jb),
        ajax: function(a, b) {
            "object" == typeof a && (b = a, a = void 0), b = b || {};
            var c, d, e, f, g, h, i, j, k = n.ajaxSetup({}, b),
                l = k.context || k,
                m = k.context && (l.nodeType || l.jquery) ? n(l) : n.event,
                o = n.Deferred(),
                p = n.Callbacks("once memory"),
                q = k.statusCode || {},
                r = {},
                s = {},
                t = 0,
                u = "canceled",
                v = {
                    readyState: 0,
                    getResponseHeader: function(a) {
                        var b;
                        if (2 === t) {
                            if (!f) {
                                f = {};
                                while (b = Db.exec(e)) f[b[1].toLowerCase()] = b[2]
                            }
                            b = f[a.toLowerCase()]
                        }
                        return null == b ? null : b
                    },
                    getAllResponseHeaders: function() {
                        return 2 === t ? e : null
                    },
                    setRequestHeader: function(a, b) {
                        var c = a.toLowerCase();
                        return t || (a = s[c] = s[c] || a, r[a] = b), this
                    },
                    overrideMimeType: function(a) {
                        return t || (k.mimeType = a), this
                    },
                    statusCode: function(a) {
                        var b;
                        if (a)
                            if (2 > t)
                                for (b in a) q[b] = [q[b], a[b]];
                            else v.always(a[v.status]);
                        return this
                    },
                    abort: function(a) {
                        var b = a || u;
                        return c && c.abort(b), x(0, b), this
                    }
                };
            if (o.promise(v).complete = p.add, v.success = v.done, v.error = v.fail, k.url = ((a || k.url || Ab) + "").replace(Bb, "").replace(Gb, zb[1] + "//"), k.type = b.method || b.type || k.method || k.type, k.dataTypes = n.trim(k.dataType || "*").toLowerCase().match(H) || [""], null == k.crossDomain && (h = Hb.exec(k.url.toLowerCase()), k.crossDomain = !(!h || h[1] === zb[1] && h[2] === zb[2] && (h[3] || ("http:" === h[1] ? "80" : "443")) === (zb[3] || ("http:" === zb[1] ? "80" : "443")))), k.data && k.processData && "string" != typeof k.data && (k.data = n.param(k.data, k.traditional)), Nb(Ib, k, b, v), 2 === t) return v;
            i = k.global, i && 0 === n.active++ && n.event.trigger("ajaxStart"), k.type = k.type.toUpperCase(), k.hasContent = !Fb.test(k.type), d = k.url, k.hasContent || (k.data && (d = k.url += (yb.test(d) ? "&" : "?") + k.data, delete k.data), k.cache === !1 && (k.url = Cb.test(d) ? d.replace(Cb, "$1_=" + xb++) : d + (yb.test(d) ? "&" : "?") + "_=" + xb++)), k.ifModified && (n.lastModified[d] && v.setRequestHeader("If-Modified-Since", n.lastModified[d]), n.etag[d] && v.setRequestHeader("If-None-Match", n.etag[d])), (k.data && k.hasContent && k.contentType !== !1 || b.contentType) && v.setRequestHeader("Content-Type", k.contentType), v.setRequestHeader("Accept", k.dataTypes[0] && k.accepts[k.dataTypes[0]] ? k.accepts[k.dataTypes[0]] + ("*" !== k.dataTypes[0] ? ", " + Kb + "; q=0.01" : "") : k.accepts["*"]);
            for (j in k.headers) v.setRequestHeader(j, k.headers[j]);
            if (k.beforeSend && (k.beforeSend.call(l, v, k) === !1 || 2 === t)) return v.abort();
            u = "abort";
            for (j in {
                    success: 1,
                    error: 1,
                    complete: 1
                }) v[j](k[j]);
            if (c = Nb(Jb, k, b, v)) {
                v.readyState = 1, i && m.trigger("ajaxSend", [v, k]), k.async && k.timeout > 0 && (g = setTimeout(function() {
                    v.abort("timeout")
                }, k.timeout));
                try {
                    t = 1, c.send(r, x)
                } catch (w) {
                    if (!(2 > t)) throw w;
                    x(-1, w)
                }
            } else x(-1, "No Transport");

            function x(a, b, f, h) {
                var j, r, s, u, w, x = b;
                2 !== t && (t = 2, g && clearTimeout(g), c = void 0, e = h || "", v.readyState = a > 0 ? 4 : 0, j = a >= 200 && 300 > a || 304 === a, f && (u = Pb(k, v, f)), u = Qb(k, u, v, j), j ? (k.ifModified && (w = v.getResponseHeader("Last-Modified"), w && (n.lastModified[d] = w), w = v.getResponseHeader("etag"), w && (n.etag[d] = w)), 204 === a || "HEAD" === k.type ? x = "nocontent" : 304 === a ? x = "notmodified" : (x = u.state, r = u.data, s = u.error, j = !s)) : (s = x, (a || !x) && (x = "error", 0 > a && (a = 0))), v.status = a, v.statusText = (b || x) + "", j ? o.resolveWith(l, [r, x, v]) : o.rejectWith(l, [v, x, s]), v.statusCode(q), q = void 0, i && m.trigger(j ? "ajaxSuccess" : "ajaxError", [v, k, j ? r : s]), p.fireWith(l, [v, x]), i && (m.trigger("ajaxComplete", [v, k]), --n.active || n.event.trigger("ajaxStop")))
            }
            return v
        },
        getJSON: function(a, b, c) {
            return n.get(a, b, c, "json")
        },
        getScript: function(a, b) {
            return n.get(a, void 0, b, "script")
        }
    }), n.each(["get", "post"], function(a, b) {
        n[b] = function(a, c, d, e) {
            return n.isFunction(c) && (e = e || d, d = c, c = void 0), n.ajax({
                url: a,
                type: b,
                dataType: e,
                data: c,
                success: d
            })
        }
    }), n.each(["ajaxStart", "ajaxStop", "ajaxComplete", "ajaxError", "ajaxSuccess", "ajaxSend"], function(a, b) {
        n.fn[b] = function(a) {
            return this.on(b, a)
        }
    }), n._evalUrl = function(a) {
        return n.ajax({
            url: a,
            type: "GET",
            dataType: "script",
            async: !1,
            global: !1,
            "throws": !0
        })
    };
    var Rb = /%20/g,
        Sb = /\[\]$/,
        Tb = /\r?\n/g,
        Ub = /^(?:submit|button|image|reset|file)$/i,
        Vb = /^(?:input|select|textarea|keygen)/i;

    function Wb(a, b, c, d) {
        var e;
        if (n.isArray(b)) n.each(b, function(b, e) {
            c || Sb.test(a) ? d(a, e) : Wb(a + "[" + ("object" == typeof e ? b : "") + "]", e, c, d)
        });
        else if (c || "object" !== n.type(b)) d(a, b);
        else
            for (e in b) Wb(a + "[" + e + "]", b[e], c, d)
    }
    n.param = function(a, b) {
        var c, d = [],
            e = function(a, b) {
                b = n.isFunction(b) ? b() : null == b ? "" : b, d[d.length] = encodeURIComponent(a) + "=" + encodeURIComponent(b)
            };
        if (void 0 === b && (b = n.ajaxSettings && n.ajaxSettings.traditional), n.isArray(a) || a.jquery && !n.isPlainObject(a)) n.each(a, function() {
            e(this.name, this.value)
        });
        else
            for (c in a) Wb(c, a[c], b, e);
        return d.join("&").replace(Rb, "+")
    }, n.fn.extend({
        serialize: function() {
            return n.param(this.serializeArray())
        },
        serializeArray: function() {
            return this.map(function() {
                var a = n.prop(this, "elements");
                return a ? n.makeArray(a) : this
            }).filter(function() {
                var a = this.type;
                return this.name && !n(this).is(":disabled") && Vb.test(this.nodeName) && !Ub.test(a) && (this.checked || !U.test(a))
            }).map(function(a, b) {
                var c = n(this).val();
                return null == c ? null : n.isArray(c) ? n.map(c, function(a) {
                    return {
                        name: b.name,
                        value: a.replace(Tb, "\r\n")
                    }
                }) : {
                    name: b.name,
                    value: c.replace(Tb, "\r\n")
                }
            }).get()
        }
    }), n.ajaxSettings.xhr = function() {
        try {
            return new XMLHttpRequest
        } catch (a) {}
    };
    var Xb = 0,
        Yb = {},
        Zb = {
            0: 200,
            1223: 204
        },
        $b = n.ajaxSettings.xhr();
    a.ActiveXObject && n(a).on("unload", function() {
        for (var a in Yb) Yb[a]()
    }), k.cors = !!$b && "withCredentials" in $b, k.ajax = $b = !!$b, n.ajaxTransport(function(a) {
        var b;
        return k.cors || $b && !a.crossDomain ? {
            send: function(c, d) {
                var e, f = a.xhr(),
                    g = ++Xb;
                if (f.open(a.type, a.url, a.async, a.username, a.password), a.xhrFields)
                    for (e in a.xhrFields) f[e] = a.xhrFields[e];
                a.mimeType && f.overrideMimeType && f.overrideMimeType(a.mimeType), a.crossDomain || c["X-Requested-With"] || (c["X-Requested-With"] = "XMLHttpRequest");
                for (e in c) f.setRequestHeader(e, c[e]);
                b = function(a) {
                    return function() {
                        b && (delete Yb[g], b = f.onload = f.onerror = null, "abort" === a ? f.abort() : "error" === a ? d(f.status, f.statusText) : d(Zb[f.status] || f.status, f.statusText, "string" == typeof f.responseText ? {
                            text: f.responseText
                        } : void 0, f.getAllResponseHeaders()))
                    }
                }, f.onload = b(), f.onerror = b("error"), b = Yb[g] = b("abort");
                try {
                    f.send(a.hasContent && a.data || null)
                } catch (h) {
                    if (b) throw h
                }
            },
            abort: function() {
                b && b()
            }
        } : void 0
    }), n.ajaxSetup({
        accepts: {
            script: "text/javascript, application/javascript, application/ecmascript, application/x-ecmascript"
        },
        contents: {
            script: /(?:java|ecma)script/
        },
        converters: {
            "text script": function(a) {
                return n.globalEval(a), a
            }
        }
    }), n.ajaxPrefilter("script", function(a) {
        void 0 === a.cache && (a.cache = !1), a.crossDomain && (a.type = "GET")
    }), n.ajaxTransport("script", function(a) {
        if (a.crossDomain) {
            var b, c;
            return {
                send: function(d, e) {
                    b = n("<script>").prop({
                        async: !0,
                        charset: a.scriptCharset,
                        src: a.url
                    }).on("load error", c = function(a) {
                        b.remove(), c = null, a && e("error" === a.type ? 404 : 200, a.type)
                    }), l.head.appendChild(b[0])
                },
                abort: function() {
                    c && c()
                }
            }
        }
    });
    var _b = [],
        ac = /(=)\?(?=&|$)|\?\?/;
    n.ajaxSetup({
        jsonp: "callback",
        jsonpCallback: function() {
            var a = _b.pop() || n.expando + "_" + xb++;
            return this[a] = !0, a
        }
    }), n.ajaxPrefilter("json jsonp", function(b, c, d) {
        var e, f, g, h = b.jsonp !== !1 && (ac.test(b.url) ? "url" : "string" == typeof b.data && !(b.contentType || "").indexOf("application/x-www-form-urlencoded") && ac.test(b.data) && "data");
        return h || "jsonp" === b.dataTypes[0] ? (e = b.jsonpCallback = n.isFunction(b.jsonpCallback) ? b.jsonpCallback() : b.jsonpCallback, h ? b[h] = b[h].replace(ac, "$1" + e) : b.jsonp !== !1 && (b.url += (yb.test(b.url) ? "&" : "?") + b.jsonp + "=" + e), b.converters["script json"] = function() {
            return g || n.error(e + " was not called"), g[0]
        }, b.dataTypes[0] = "json", f = a[e], a[e] = function() {
            g = arguments
        }, d.always(function() {
            a[e] = f, b[e] && (b.jsonpCallback = c.jsonpCallback, _b.push(e)), g && n.isFunction(f) && f(g[0]), g = f = void 0
        }), "script") : void 0
    }), n.parseHTML = function(a, b, c) {
        if (!a || "string" != typeof a) return null;
        "boolean" == typeof b && (c = b, b = !1), b = b || l;
        var d = y.exec(a),
            e = !c && [];
        return d ? [b.createElement(d[1])] : (d = n.buildFragment([a], b, e), e && e.length && n(e).remove(), n.merge([], d.childNodes))
    };
    var bc = n.fn.load;
    n.fn.load = function(a, b, c) {
        if ("string" != typeof a && bc) return bc.apply(this, arguments);
        var d, e, f, g = this,
            h = a.indexOf(" ");
        return h >= 0 && (d = n.trim(a.slice(h)), a = a.slice(0, h)), n.isFunction(b) ? (c = b, b = void 0) : b && "object" == typeof b && (e = "POST"), g.length > 0 && n.ajax({
            url: a,
            type: e,
            dataType: "html",
            data: b
        }).done(function(a) {
            f = arguments, g.html(d ? n("<div>").append(n.parseHTML(a)).find(d) : a)
        }).complete(c && function(a, b) {
            g.each(c, f || [a.responseText, b, a])
        }), this
    }, "function" == typeof define && define.amd && define("jquery", [], function() {
        return n
    });
    var cc = a.jQuery,
        dc = a.$;
    return n.noConflict = function(b) {
        return a.$ === n && (a.$ = dc), b && a.jQuery === n && (a.jQuery = cc), n
    }, typeof b === V && (a.jQuery = a.$ = n), n
});
//# sourceMappingURL=jquery.min.map
/*!
 * Knockout JavaScript library v3.4.0
 * (c) Steven Sanderson - http://knockoutjs.com/
 * License: MIT (http://www.opensource.org/licenses/mit-license.php)
 */

(function() {
    (function(n) {
        var x = this || (0, eval)("this"),
            u = x.document,
            M = x.navigator,
            v = x.jQuery,
            F = x.JSON;
        (function(n) {
            "function" === typeof define && define.amd ? define(["exports", "require"], n) : "object" === typeof exports && "object" === typeof module ? n(module.exports || exports) : n(x.ko = {})
        })(function(N, O) {
            function J(a, c) {
                return null === a || typeof a in T ? a === c : !1
            }

            function U(b, c) {
                var d;
                return function() {
                    d || (d = a.a.setTimeout(function() {
                        d = n;
                        b()
                    }, c))
                }
            }

            function V(b, c) {
                var d;
                return function() {
                    clearTimeout(d);
                    d = a.a.setTimeout(b, c)
                }
            }

            function W(a,
                c) {
                c && c !== I ? "beforeChange" === c ? this.Kb(a) : this.Ha(a, c) : this.Lb(a)
            }

            function X(a, c) {
                null !== c && c.k && c.k()
            }

            function Y(a, c) {
                var d = this.Hc,
                    e = d[s];
                e.R || (this.lb && this.Ma[c] ? (d.Pb(c, a, this.Ma[c]), this.Ma[c] = null, --this.lb) : e.r[c] || d.Pb(c, a, e.s ? {
                    ia: a
                } : d.uc(a)))
            }

            function K(b, c, d, e) {
                a.d[b] = {
                    init: function(b, g, k, l, m) {
                        var h, r;
                        a.m(function() {
                            var q = a.a.c(g()),
                                p = !d !== !q,
                                A = !r;
                            if (A || c || p !== h) A && a.va.Aa() && (r = a.a.ua(a.f.childNodes(b), !0)), p ? (A || a.f.da(b, a.a.ua(r)), a.eb(e ? e(m, q) : m, b)) : a.f.xa(b), h = p
                        }, null, {
                            i: b
                        });
                        return {
                            controlsDescendantBindings: !0
                        }
                    }
                };
                a.h.ta[b] = !1;
                a.f.Z[b] = !0
            }
            var a = "undefined" !== typeof N ? N : {};
            a.b = function(b, c) {
                for (var d = b.split("."), e = a, f = 0; f < d.length - 1; f++) e = e[d[f]];
                e[d[d.length - 1]] = c
            };
            a.G = function(a, c, d) {
                a[c] = d
            };
            a.version = "3.4.0";
            a.b("version", a.version);
            a.options = {
                deferUpdates: !1,
                useOnlyNativeEvents: !1
            };
            a.a = function() {
                function b(a, b) {
                    for (var c in a) a.hasOwnProperty(c) && b(c, a[c])
                }

                function c(a, b) {
                    if (b)
                        for (var c in b) b.hasOwnProperty(c) && (a[c] = b[c]);
                    return a
                }

                function d(a, b) {
                    a.__proto__ = b;
                    return a
                }

                function e(b, c, d, e) {
                    var h = b[c].match(r) || [];
                    a.a.q(d.match(r), function(b) {
                        a.a.pa(h, b, e)
                    });
                    b[c] = h.join(" ")
                }
                var f = {
                    __proto__: []
                }
                instanceof Array, g = "function" === typeof Symbol, k = {}, l = {};
                k[M && /Firefox\/2/i.test(M.userAgent) ? "KeyboardEvent" : "UIEvents"] = ["keyup", "keydown", "keypress"];
                k.MouseEvents = "click dblclick mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave".split(" ");
                b(k, function(a, b) {
                    if (b.length)
                        for (var c = 0, d = b.length; c < d; c++) l[b[c]] = a
                });
                var m = {
                        propertychange: !0
                    },
                    h = u && function() {
                        for (var a = 3, b = u.createElement("div"), c =
                                b.getElementsByTagName("i"); b.innerHTML = "\x3c!--[if gt IE " + ++a + "]><i></i><![endif]--\x3e", c[0];);
                        return 4 < a ? a : n
                    }(),
                    r = /\S+/g;
                return {
                    cc: ["authenticity_token", /^__RequestVerificationToken(_.*)?$/],
                    q: function(a, b) {
                        for (var c = 0, d = a.length; c < d; c++) b(a[c], c)
                    },
                    o: function(a, b) {
                        if ("function" == typeof Array.prototype.indexOf) return Array.prototype.indexOf.call(a, b);
                        for (var c = 0, d = a.length; c < d; c++)
                            if (a[c] === b) return c;
                        return -1
                    },
                    Sb: function(a, b, c) {
                        for (var d = 0, e = a.length; d < e; d++)
                            if (b.call(c, a[d], d)) return a[d];
                        return null
                    },
                    La: function(b, c) {
                        var d = a.a.o(b, c);
                        0 < d ? b.splice(d, 1) : 0 === d && b.shift()
                    },
                    Tb: function(b) {
                        b = b || [];
                        for (var c = [], d = 0, e = b.length; d < e; d++) 0 > a.a.o(c, b[d]) && c.push(b[d]);
                        return c
                    },
                    fb: function(a, b) {
                        a = a || [];
                        for (var c = [], d = 0, e = a.length; d < e; d++) c.push(b(a[d], d));
                        return c
                    },
                    Ka: function(a, b) {
                        a = a || [];
                        for (var c = [], d = 0, e = a.length; d < e; d++) b(a[d], d) && c.push(a[d]);
                        return c
                    },
                    ra: function(a, b) {
                        if (b instanceof Array) a.push.apply(a, b);
                        else
                            for (var c = 0, d = b.length; c < d; c++) a.push(b[c]);
                        return a
                    },
                    pa: function(b, c, d) {
                        var e =
                            a.a.o(a.a.zb(b), c);
                        0 > e ? d && b.push(c) : d || b.splice(e, 1)
                    },
                    ka: f,
                    extend: c,
                    Xa: d,
                    Ya: f ? d : c,
                    D: b,
                    Ca: function(a, b) {
                        if (!a) return a;
                        var c = {},
                            d;
                        for (d in a) a.hasOwnProperty(d) && (c[d] = b(a[d], d, a));
                        return c
                    },
                    ob: function(b) {
                        for (; b.firstChild;) a.removeNode(b.firstChild)
                    },
                    jc: function(b) {
                        b = a.a.V(b);
                        for (var c = (b[0] && b[0].ownerDocument || u).createElement("div"), d = 0, e = b.length; d < e; d++) c.appendChild(a.$(b[d]));
                        return c
                    },
                    ua: function(b, c) {
                        for (var d = 0, e = b.length, h = []; d < e; d++) {
                            var m = b[d].cloneNode(!0);
                            h.push(c ? a.$(m) : m)
                        }
                        return h
                    },
                    da: function(b, c) {
                        a.a.ob(b);
                        if (c)
                            for (var d = 0, e = c.length; d < e; d++) b.appendChild(c[d])
                    },
                    qc: function(b, c) {
                        var d = b.nodeType ? [b] : b;
                        if (0 < d.length) {
                            for (var e = d[0], h = e.parentNode, m = 0, l = c.length; m < l; m++) h.insertBefore(c[m], e);
                            m = 0;
                            for (l = d.length; m < l; m++) a.removeNode(d[m])
                        }
                    },
                    za: function(a, b) {
                        if (a.length) {
                            for (b = 8 === b.nodeType && b.parentNode || b; a.length && a[0].parentNode !== b;) a.splice(0, 1);
                            for (; 1 < a.length && a[a.length - 1].parentNode !== b;) a.length--;
                            if (1 < a.length) {
                                var c = a[0],
                                    d = a[a.length - 1];
                                for (a.length = 0; c !== d;) a.push(c),
                                    c = c.nextSibling;
                                a.push(d)
                            }
                        }
                        return a
                    },
                    sc: function(a, b) {
                        7 > h ? a.setAttribute("selected", b) : a.selected = b
                    },
                    $a: function(a) {
                        return null === a || a === n ? "" : a.trim ? a.trim() : a.toString().replace(/^[\s\xa0]+|[\s\xa0]+$/g, "")
                    },
                    nd: function(a, b) {
                        a = a || "";
                        return b.length > a.length ? !1 : a.substring(0, b.length) === b
                    },
                    Mc: function(a, b) {
                        if (a === b) return !0;
                        if (11 === a.nodeType) return !1;
                        if (b.contains) return b.contains(3 === a.nodeType ? a.parentNode : a);
                        if (b.compareDocumentPosition) return 16 == (b.compareDocumentPosition(a) & 16);
                        for (; a && a !=
                            b;) a = a.parentNode;
                        return !!a
                    },
                    nb: function(b) {
                        return a.a.Mc(b, b.ownerDocument.documentElement)
                    },
                    Qb: function(b) {
                        return !!a.a.Sb(b, a.a.nb)
                    },
                    A: function(a) {
                        return a && a.tagName && a.tagName.toLowerCase()
                    },
                    Wb: function(b) {
                        return a.onError ? function() {
                            try {
                                return b.apply(this, arguments)
                            } catch (c) {
                                throw a.onError && a.onError(c), c;
                            }
                        } : b
                    },
                    setTimeout: function(b, c) {
                        return setTimeout(a.a.Wb(b), c)
                    },
                    $b: function(b) {
                        setTimeout(function() {
                            a.onError && a.onError(b);
                            throw b;
                        }, 0)
                    },
                    p: function(b, c, d) {
                        var e = a.a.Wb(d);
                        d = h && m[c];
                        if (a.options.useOnlyNativeEvents ||
                            d || !v)
                            if (d || "function" != typeof b.addEventListener)
                                if ("undefined" != typeof b.attachEvent) {
                                    var l = function(a) {
                                            e.call(b, a)
                                        },
                                        f = "on" + c;
                                    b.attachEvent(f, l);
                                    a.a.F.oa(b, function() {
                                        b.detachEvent(f, l)
                                    })
                                } else throw Error("Browser doesn't support addEventListener or attachEvent");
                        else b.addEventListener(c, e, !1);
                        else v(b).bind(c, e)
                    },
                    Da: function(b, c) {
                        if (!b || !b.nodeType) throw Error("element must be a DOM node when calling triggerEvent");
                        var d;
                        "input" === a.a.A(b) && b.type && "click" == c.toLowerCase() ? (d = b.type, d = "checkbox" ==
                            d || "radio" == d) : d = !1;
                        if (a.options.useOnlyNativeEvents || !v || d)
                            if ("function" == typeof u.createEvent)
                                if ("function" == typeof b.dispatchEvent) d = u.createEvent(l[c] || "HTMLEvents"), d.initEvent(c, !0, !0, x, 0, 0, 0, 0, 0, !1, !1, !1, !1, 0, b), b.dispatchEvent(d);
                                else throw Error("The supplied element doesn't support dispatchEvent");
                        else if (d && b.click) b.click();
                        else if ("undefined" != typeof b.fireEvent) b.fireEvent("on" + c);
                        else throw Error("Browser doesn't support triggering events");
                        else v(b).trigger(c)
                    },
                    c: function(b) {
                        return a.H(b) ?
                            b() : b
                    },
                    zb: function(b) {
                        return a.H(b) ? b.t() : b
                    },
                    bb: function(b, c, d) {
                        var h;
                        c && ("object" === typeof b.classList ? (h = b.classList[d ? "add" : "remove"], a.a.q(c.match(r), function(a) {
                            h.call(b.classList, a)
                        })) : "string" === typeof b.className.baseVal ? e(b.className, "baseVal", c, d) : e(b, "className", c, d))
                    },
                    Za: function(b, c) {
                        var d = a.a.c(c);
                        if (null === d || d === n) d = "";
                        var e = a.f.firstChild(b);
                        !e || 3 != e.nodeType || a.f.nextSibling(e) ? a.f.da(b, [b.ownerDocument.createTextNode(d)]) : e.data = d;
                        a.a.Rc(b)
                    },
                    rc: function(a, b) {
                        a.name = b;
                        if (7 >= h) try {
                            a.mergeAttributes(u.createElement("<input name='" +
                                a.name + "'/>"), !1)
                        } catch (c) {}
                    },
                    Rc: function(a) {
                        9 <= h && (a = 1 == a.nodeType ? a : a.parentNode, a.style && (a.style.zoom = a.style.zoom))
                    },
                    Nc: function(a) {
                        if (h) {
                            var b = a.style.width;
                            a.style.width = 0;
                            a.style.width = b
                        }
                    },
                    hd: function(b, c) {
                        b = a.a.c(b);
                        c = a.a.c(c);
                        for (var d = [], e = b; e <= c; e++) d.push(e);
                        return d
                    },
                    V: function(a) {
                        for (var b = [], c = 0, d = a.length; c < d; c++) b.push(a[c]);
                        return b
                    },
                    Yb: function(a) {
                        return g ? Symbol(a) : a
                    },
                    rd: 6 === h,
                    sd: 7 === h,
                    C: h,
                    ec: function(b, c) {
                        for (var d = a.a.V(b.getElementsByTagName("input")).concat(a.a.V(b.getElementsByTagName("textarea"))),
                                e = "string" == typeof c ? function(a) {
                                    return a.name === c
                                } : function(a) {
                                    return c.test(a.name)
                                }, h = [], m = d.length - 1; 0 <= m; m--) e(d[m]) && h.push(d[m]);
                        return h
                    },
                    ed: function(b) {
                        return "string" == typeof b && (b = a.a.$a(b)) ? F && F.parse ? F.parse(b) : (new Function("return " + b))() : null
                    },
                    Eb: function(b, c, d) {
                        if (!F || !F.stringify) throw Error("Cannot find JSON.stringify(). Some browsers (e.g., IE < 8) don't support it natively, but you can overcome this by adding a script reference to json2.js, downloadable from http://www.json.org/json2.js");
                        return F.stringify(a.a.c(b), c, d)
                    },
                    fd: function(c, d, e) {
                        e = e || {};
                        var h = e.params || {},
                            m = e.includeFields || this.cc,
                            l = c;
                        if ("object" == typeof c && "form" === a.a.A(c))
                            for (var l = c.action, f = m.length - 1; 0 <= f; f--)
                                for (var g = a.a.ec(c, m[f]), k = g.length - 1; 0 <= k; k--) h[g[k].name] = g[k].value;
                        d = a.a.c(d);
                        var r = u.createElement("form");
                        r.style.display = "none";
                        r.action = l;
                        r.method = "post";
                        for (var n in d) c = u.createElement("input"), c.type = "hidden", c.name = n, c.value = a.a.Eb(a.a.c(d[n])), r.appendChild(c);
                        b(h, function(a, b) {
                            var c = u.createElement("input");
                            c.type = "hidden";
                            c.name = a;
                            c.value = b;
                            r.appendChild(c)
                        });
                        u.body.appendChild(r);
                        e.submitter ? e.submitter(r) : r.submit();
                        setTimeout(function() {
                            r.parentNode.removeChild(r)
                        }, 0)
                    }
                }
            }();
            a.b("utils", a.a);
            a.b("utils.arrayForEach", a.a.q);
            a.b("utils.arrayFirst", a.a.Sb);
            a.b("utils.arrayFilter", a.a.Ka);
            a.b("utils.arrayGetDistinctValues", a.a.Tb);
            a.b("utils.arrayIndexOf", a.a.o);
            a.b("utils.arrayMap", a.a.fb);
            a.b("utils.arrayPushAll", a.a.ra);
            a.b("utils.arrayRemoveItem", a.a.La);
            a.b("utils.extend", a.a.extend);
            a.b("utils.fieldsIncludedWithJsonPost",
                a.a.cc);
            a.b("utils.getFormFields", a.a.ec);
            a.b("utils.peekObservable", a.a.zb);
            a.b("utils.postJson", a.a.fd);
            a.b("utils.parseJson", a.a.ed);
            a.b("utils.registerEventHandler", a.a.p);
            a.b("utils.stringifyJson", a.a.Eb);
            a.b("utils.range", a.a.hd);
            a.b("utils.toggleDomNodeCssClass", a.a.bb);
            a.b("utils.triggerEvent", a.a.Da);
            a.b("utils.unwrapObservable", a.a.c);
            a.b("utils.objectForEach", a.a.D);
            a.b("utils.addOrRemoveItem", a.a.pa);
            a.b("utils.setTextContent", a.a.Za);
            a.b("unwrap", a.a.c);
            Function.prototype.bind || (Function.prototype.bind =
                function(a) {
                    var c = this;
                    if (1 === arguments.length) return function() {
                        return c.apply(a, arguments)
                    };
                    var d = Array.prototype.slice.call(arguments, 1);
                    return function() {
                        var e = d.slice(0);
                        e.push.apply(e, arguments);
                        return c.apply(a, e)
                    }
                });
            a.a.e = new function() {
                function a(b, g) {
                    var k = b[d];
                    if (!k || "null" === k || !e[k]) {
                        if (!g) return n;
                        k = b[d] = "ko" + c++;
                        e[k] = {}
                    }
                    return e[k]
                }
                var c = 0,
                    d = "__ko__" + (new Date).getTime(),
                    e = {};
                return {
                    get: function(c, d) {
                        var e = a(c, !1);
                        return e === n ? n : e[d]
                    },
                    set: function(c, d, e) {
                        if (e !== n || a(c, !1) !== n) a(c, !0)[d] =
                            e
                    },
                    clear: function(a) {
                        var b = a[d];
                        return b ? (delete e[b], a[d] = null, !0) : !1
                    },
                    I: function() {
                        return c++ + d
                    }
                }
            };
            a.b("utils.domData", a.a.e);
            a.b("utils.domData.clear", a.a.e.clear);
            a.a.F = new function() {
                function b(b, c) {
                    var e = a.a.e.get(b, d);
                    e === n && c && (e = [], a.a.e.set(b, d, e));
                    return e
                }

                function c(d) {
                    var e = b(d, !1);
                    if (e)
                        for (var e = e.slice(0), l = 0; l < e.length; l++) e[l](d);
                    a.a.e.clear(d);
                    a.a.F.cleanExternalData(d);
                    if (f[d.nodeType])
                        for (e = d.firstChild; d = e;) e = d.nextSibling, 8 === d.nodeType && c(d)
                }
                var d = a.a.e.I(),
                    e = {
                        1: !0,
                        8: !0,
                        9: !0
                    },
                    f = {
                        1: !0,
                        9: !0
                    };
                return {
                    oa: function(a, c) {
                        if ("function" != typeof c) throw Error("Callback must be a function");
                        b(a, !0).push(c)
                    },
                    pc: function(c, e) {
                        var l = b(c, !1);
                        l && (a.a.La(l, e), 0 == l.length && a.a.e.set(c, d, n))
                    },
                    $: function(b) {
                        if (e[b.nodeType] && (c(b), f[b.nodeType])) {
                            var d = [];
                            a.a.ra(d, b.getElementsByTagName("*"));
                            for (var l = 0, m = d.length; l < m; l++) c(d[l])
                        }
                        return b
                    },
                    removeNode: function(b) {
                        a.$(b);
                        b.parentNode && b.parentNode.removeChild(b)
                    },
                    cleanExternalData: function(a) {
                        v && "function" == typeof v.cleanData && v.cleanData([a])
                    }
                }
            };
            a.$ = a.a.F.$;
            a.removeNode = a.a.F.removeNode;
            a.b("cleanNode", a.$);
            a.b("removeNode", a.removeNode);
            a.b("utils.domNodeDisposal", a.a.F);
            a.b("utils.domNodeDisposal.addDisposeCallback", a.a.F.oa);
            a.b("utils.domNodeDisposal.removeDisposeCallback", a.a.F.pc);
            (function() {
                var b = [0, "", ""],
                    c = [1, "<table>", "</table>"],
                    d = [3, "<table><tbody><tr>", "</tr></tbody></table>"],
                    e = [1, "<select multiple='multiple'>", "</select>"],
                    f = {
                        thead: c,
                        tbody: c,
                        tfoot: c,
                        tr: [2, "<table><tbody>", "</tbody></table>"],
                        td: d,
                        th: d,
                        option: e,
                        optgroup: e
                    },
                    g = 8 >= a.a.C;
                a.a.ma = function(c, d) {
                    var e;
                    if (v)
                        if (v.parseHTML) e = v.parseHTML(c, d) || [];
                        else {
                            if ((e = v.clean([c], d)) && e[0]) {
                                for (var h = e[0]; h.parentNode && 11 !== h.parentNode.nodeType;) h = h.parentNode;
                                h.parentNode && h.parentNode.removeChild(h)
                            }
                        }
                    else {
                        (e = d) || (e = u);
                        var h = e.parentWindow || e.defaultView || x,
                            r = a.a.$a(c).toLowerCase(),
                            q = e.createElement("div"),
                            p;
                        p = (r = r.match(/^<([a-z]+)[ >]/)) && f[r[1]] || b;
                        r = p[0];
                        p = "ignored<div>" + p[1] + c + p[2] + "</div>";
                        "function" == typeof h.innerShiv ? q.appendChild(h.innerShiv(p)) : (g && e.appendChild(q),
                            q.innerHTML = p, g && q.parentNode.removeChild(q));
                        for (; r--;) q = q.lastChild;
                        e = a.a.V(q.lastChild.childNodes)
                    }
                    return e
                };
                a.a.Cb = function(b, c) {
                    a.a.ob(b);
                    c = a.a.c(c);
                    if (null !== c && c !== n)
                        if ("string" != typeof c && (c = c.toString()), v) v(b).html(c);
                        else
                            for (var d = a.a.ma(c, b.ownerDocument), e = 0; e < d.length; e++) b.appendChild(d[e])
                }
            })();
            a.b("utils.parseHtmlFragment", a.a.ma);
            a.b("utils.setHtml", a.a.Cb);
            a.M = function() {
                function b(c, e) {
                    if (c)
                        if (8 == c.nodeType) {
                            var f = a.M.lc(c.nodeValue);
                            null != f && e.push({
                                Lc: c,
                                cd: f
                            })
                        } else if (1 == c.nodeType)
                        for (var f =
                                0, g = c.childNodes, k = g.length; f < k; f++) b(g[f], e)
                }
                var c = {};
                return {
                    wb: function(a) {
                        if ("function" != typeof a) throw Error("You can only pass a function to ko.memoization.memoize()");
                        var b = (4294967296 * (1 + Math.random()) | 0).toString(16).substring(1) + (4294967296 * (1 + Math.random()) | 0).toString(16).substring(1);
                        c[b] = a;
                        return "\x3c!--[ko_memo:" + b + "]--\x3e"
                    },
                    xc: function(a, b) {
                        var f = c[a];
                        if (f === n) throw Error("Couldn't find any memo with ID " + a + ". Perhaps it's already been unmemoized.");
                        try {
                            return f.apply(null, b || []),
                                !0
                        } finally {
                            delete c[a]
                        }
                    },
                    yc: function(c, e) {
                        var f = [];
                        b(c, f);
                        for (var g = 0, k = f.length; g < k; g++) {
                            var l = f[g].Lc,
                                m = [l];
                            e && a.a.ra(m, e);
                            a.M.xc(f[g].cd, m);
                            l.nodeValue = "";
                            l.parentNode && l.parentNode.removeChild(l)
                        }
                    },
                    lc: function(a) {
                        return (a = a.match(/^\[ko_memo\:(.*?)\]$/)) ? a[1] : null
                    }
                }
            }();
            a.b("memoization", a.M);
            a.b("memoization.memoize", a.M.wb);
            a.b("memoization.unmemoize", a.M.xc);
            a.b("memoization.parseMemoText", a.M.lc);
            a.b("memoization.unmemoizeDomNodeAndDescendants", a.M.yc);
            a.Y = function() {
                function b() {
                    if (e)
                        for (var b =
                                e, c = 0, m; g < e;)
                            if (m = d[g++]) {
                                if (g > b) {
                                    if (5E3 <= ++c) {
                                        g = e;
                                        a.a.$b(Error("'Too much recursion' after processing " + c + " task groups."));
                                        break
                                    }
                                    b = e
                                }
                                try {
                                    m()
                                } catch (h) {
                                    a.a.$b(h)
                                }
                            }
                }

                function c() {
                    b();
                    g = e = d.length = 0
                }
                var d = [],
                    e = 0,
                    f = 1,
                    g = 0;
                return {
                    scheduler: x.MutationObserver ? function(a) {
                        var b = u.createElement("div");
                        (new MutationObserver(a)).observe(b, {
                            attributes: !0
                        });
                        return function() {
                            b.classList.toggle("foo")
                        }
                    }(c) : u && "onreadystatechange" in u.createElement("script") ? function(a) {
                        var b = u.createElement("script");
                        b.onreadystatechange =
                            function() {
                                b.onreadystatechange = null;
                                u.documentElement.removeChild(b);
                                b = null;
                                a()
                            };
                        u.documentElement.appendChild(b)
                    } : function(a) {
                        setTimeout(a, 0)
                    },
                    Wa: function(b) {
                        e || a.Y.scheduler(c);
                        d[e++] = b;
                        return f++
                    },
                    cancel: function(a) {
                        a -= f - e;
                        a >= g && a < e && (d[a] = null)
                    },
                    resetForTesting: function() {
                        var a = e - g;
                        g = e = d.length = 0;
                        return a
                    },
                    md: b
                }
            }();
            a.b("tasks", a.Y);
            a.b("tasks.schedule", a.Y.Wa);
            a.b("tasks.runEarly", a.Y.md);
            a.ya = {
                throttle: function(b, c) {
                    b.throttleEvaluation = c;
                    var d = null;
                    return a.B({
                        read: b,
                        write: function(e) {
                            clearTimeout(d);
                            d = a.a.setTimeout(function() {
                                b(e)
                            }, c)
                        }
                    })
                },
                rateLimit: function(a, c) {
                    var d, e, f;
                    "number" == typeof c ? d = c : (d = c.timeout, e = c.method);
                    a.cb = !1;
                    f = "notifyWhenChangesStop" == e ? V : U;
                    a.Ta(function(a) {
                        return f(a, d)
                    })
                },
                deferred: function(b, c) {
                    if (!0 !== c) throw Error("The 'deferred' extender only accepts the value 'true', because it is not supported to turn deferral off once enabled.");
                    b.cb || (b.cb = !0, b.Ta(function(c) {
                        var e;
                        return function() {
                            a.Y.cancel(e);
                            e = a.Y.Wa(c);
                            b.notifySubscribers(n, "dirty")
                        }
                    }))
                },
                notify: function(a, c) {
                    a.equalityComparer =
                        "always" == c ? null : J
                }
            };
            var T = {
                undefined: 1,
                "boolean": 1,
                number: 1,
                string: 1
            };
            a.b("extenders", a.ya);
            a.vc = function(b, c, d) {
                this.ia = b;
                this.gb = c;
                this.Kc = d;
                this.R = !1;
                a.G(this, "dispose", this.k)
            };
            a.vc.prototype.k = function() {
                this.R = !0;
                this.Kc()
            };
            a.J = function() {
                a.a.Ya(this, D);
                D.rb(this)
            };
            var I = "change",
                D = {
                    rb: function(a) {
                        a.K = {};
                        a.Nb = 1
                    },
                    X: function(b, c, d) {
                        var e = this;
                        d = d || I;
                        var f = new a.vc(e, c ? b.bind(c) : b, function() {
                            a.a.La(e.K[d], f);
                            e.Ia && e.Ia(d)
                        });
                        e.sa && e.sa(d);
                        e.K[d] || (e.K[d] = []);
                        e.K[d].push(f);
                        return f
                    },
                    notifySubscribers: function(b,
                        c) {
                        c = c || I;
                        c === I && this.zc();
                        if (this.Pa(c)) try {
                            a.l.Ub();
                            for (var d = this.K[c].slice(0), e = 0, f; f = d[e]; ++e) f.R || f.gb(b)
                        } finally {
                            a.l.end()
                        }
                    },
                    Na: function() {
                        return this.Nb
                    },
                    Uc: function(a) {
                        return this.Na() !== a
                    },
                    zc: function() {
                        ++this.Nb
                    },
                    Ta: function(b) {
                        var c = this,
                            d = a.H(c),
                            e, f, g;
                        c.Ha || (c.Ha = c.notifySubscribers, c.notifySubscribers = W);
                        var k = b(function() {
                            c.Mb = !1;
                            d && g === c && (g = c());
                            e = !1;
                            c.tb(f, g) && c.Ha(f = g)
                        });
                        c.Lb = function(a) {
                            c.Mb = e = !0;
                            g = a;
                            k()
                        };
                        c.Kb = function(a) {
                            e || (f = a, c.Ha(a, "beforeChange"))
                        }
                    },
                    Pa: function(a) {
                        return this.K[a] &&
                            this.K[a].length
                    },
                    Sc: function(b) {
                        if (b) return this.K[b] && this.K[b].length || 0;
                        var c = 0;
                        a.a.D(this.K, function(a, b) {
                            "dirty" !== a && (c += b.length)
                        });
                        return c
                    },
                    tb: function(a, c) {
                        return !this.equalityComparer || !this.equalityComparer(a, c)
                    },
                    extend: function(b) {
                        var c = this;
                        b && a.a.D(b, function(b, e) {
                            var f = a.ya[b];
                            "function" == typeof f && (c = f(c, e) || c)
                        });
                        return c
                    }
                };
            a.G(D, "subscribe", D.X);
            a.G(D, "extend", D.extend);
            a.G(D, "getSubscriptionsCount", D.Sc);
            a.a.ka && a.a.Xa(D, Function.prototype);
            a.J.fn = D;
            a.hc = function(a) {
                return null !=
                    a && "function" == typeof a.X && "function" == typeof a.notifySubscribers
            };
            a.b("subscribable", a.J);
            a.b("isSubscribable", a.hc);
            a.va = a.l = function() {
                function b(a) {
                    d.push(e);
                    e = a
                }

                function c() {
                    e = d.pop()
                }
                var d = [],
                    e, f = 0;
                return {
                    Ub: b,
                    end: c,
                    oc: function(b) {
                        if (e) {
                            if (!a.hc(b)) throw Error("Only subscribable things can act as dependencies");
                            e.gb.call(e.Gc, b, b.Cc || (b.Cc = ++f))
                        }
                    },
                    w: function(a, d, e) {
                        try {
                            return b(), a.apply(d, e || [])
                        } finally {
                            c()
                        }
                    },
                    Aa: function() {
                        if (e) return e.m.Aa()
                    },
                    Sa: function() {
                        if (e) return e.Sa
                    }
                }
            }();
            a.b("computedContext",
                a.va);
            a.b("computedContext.getDependenciesCount", a.va.Aa);
            a.b("computedContext.isInitial", a.va.Sa);
            a.b("ignoreDependencies", a.qd = a.l.w);
            var E = a.a.Yb("_latestValue");
            a.N = function(b) {
                function c() {
                    if (0 < arguments.length) return c.tb(c[E], arguments[0]) && (c.ga(), c[E] = arguments[0], c.fa()), this;
                    a.l.oc(c);
                    return c[E]
                }
                c[E] = b;
                a.a.ka || a.a.extend(c, a.J.fn);
                a.J.fn.rb(c);
                a.a.Ya(c, B);
                a.options.deferUpdates && a.ya.deferred(c, !0);
                return c
            };
            var B = {
                equalityComparer: J,
                t: function() {
                    return this[E]
                },
                fa: function() {
                    this.notifySubscribers(this[E])
                },
                ga: function() {
                    this.notifySubscribers(this[E], "beforeChange")
                }
            };
            a.a.ka && a.a.Xa(B, a.J.fn);
            var H = a.N.gd = "__ko_proto__";
            B[H] = a.N;
            a.Oa = function(b, c) {
                return null === b || b === n || b[H] === n ? !1 : b[H] === c ? !0 : a.Oa(b[H], c)
            };
            a.H = function(b) {
                return a.Oa(b, a.N)
            };
            a.Ba = function(b) {
                return "function" == typeof b && b[H] === a.N || "function" == typeof b && b[H] === a.B && b.Vc ? !0 : !1
            };
            a.b("observable", a.N);
            a.b("isObservable", a.H);
            a.b("isWriteableObservable", a.Ba);
            a.b("isWritableObservable", a.Ba);
            a.b("observable.fn", B);
            a.G(B, "peek", B.t);
            a.G(B,
                "valueHasMutated", B.fa);
            a.G(B, "valueWillMutate", B.ga);
            a.la = function(b) {
                b = b || [];
                if ("object" != typeof b || !("length" in b)) throw Error("The argument passed when initializing an observable array must be an array, or null, or undefined.");
                b = a.N(b);
                a.a.Ya(b, a.la.fn);
                return b.extend({
                    trackArrayChanges: !0
                })
            };
            a.la.fn = {
                remove: function(b) {
                    for (var c = this.t(), d = [], e = "function" != typeof b || a.H(b) ? function(a) {
                            return a === b
                        } : b, f = 0; f < c.length; f++) {
                        var g = c[f];
                        e(g) && (0 === d.length && this.ga(), d.push(g), c.splice(f, 1), f--)
                    }
                    d.length &&
                        this.fa();
                    return d
                },
                removeAll: function(b) {
                    if (b === n) {
                        var c = this.t(),
                            d = c.slice(0);
                        this.ga();
                        c.splice(0, c.length);
                        this.fa();
                        return d
                    }
                    return b ? this.remove(function(c) {
                        return 0 <= a.a.o(b, c)
                    }) : []
                },
                destroy: function(b) {
                    var c = this.t(),
                        d = "function" != typeof b || a.H(b) ? function(a) {
                            return a === b
                        } : b;
                    this.ga();
                    for (var e = c.length - 1; 0 <= e; e--) d(c[e]) && (c[e]._destroy = !0);
                    this.fa()
                },
                destroyAll: function(b) {
                    return b === n ? this.destroy(function() {
                        return !0
                    }) : b ? this.destroy(function(c) {
                        return 0 <= a.a.o(b, c)
                    }) : []
                },
                indexOf: function(b) {
                    var c =
                        this();
                    return a.a.o(c, b)
                },
                replace: function(a, c) {
                    var d = this.indexOf(a);
                    0 <= d && (this.ga(), this.t()[d] = c, this.fa())
                }
            };
            a.a.ka && a.a.Xa(a.la.fn, a.N.fn);
            a.a.q("pop push reverse shift sort splice unshift".split(" "), function(b) {
                a.la.fn[b] = function() {
                    var a = this.t();
                    this.ga();
                    this.Vb(a, b, arguments);
                    var d = a[b].apply(a, arguments);
                    this.fa();
                    return d === a ? this : d
                }
            });
            a.a.q(["slice"], function(b) {
                a.la.fn[b] = function() {
                    var a = this();
                    return a[b].apply(a, arguments)
                }
            });
            a.b("observableArray", a.la);
            a.ya.trackArrayChanges = function(b,
                c) {
                function d() {
                    if (!e) {
                        e = !0;
                        var c = b.notifySubscribers;
                        b.notifySubscribers = function(a, b) {
                            b && b !== I || ++k;
                            return c.apply(this, arguments)
                        };
                        var d = [].concat(b.t() || []);
                        f = null;
                        g = b.X(function(c) {
                            c = [].concat(c || []);
                            if (b.Pa("arrayChange")) {
                                var e;
                                if (!f || 1 < k) f = a.a.ib(d, c, b.hb);
                                e = f
                            }
                            d = c;
                            f = null;
                            k = 0;
                            e && e.length && b.notifySubscribers(e, "arrayChange")
                        })
                    }
                }
                b.hb = {};
                c && "object" == typeof c && a.a.extend(b.hb, c);
                b.hb.sparse = !0;
                if (!b.Vb) {
                    var e = !1,
                        f = null,
                        g, k = 0,
                        l = b.sa,
                        m = b.Ia;
                    b.sa = function(a) {
                        l && l.call(b, a);
                        "arrayChange" === a && d()
                    };
                    b.Ia = function(a) {
                        m && m.call(b, a);
                        "arrayChange" !== a || b.Pa("arrayChange") || (g.k(), e = !1)
                    };
                    b.Vb = function(b, c, d) {
                        function m(a, b, c) {
                            return l[l.length] = {
                                status: a,
                                value: b,
                                index: c
                            }
                        }
                        if (e && !k) {
                            var l = [],
                                g = b.length,
                                t = d.length,
                                G = 0;
                            switch (c) {
                                case "push":
                                    G = g;
                                case "unshift":
                                    for (c = 0; c < t; c++) m("added", d[c], G + c);
                                    break;
                                case "pop":
                                    G = g - 1;
                                case "shift":
                                    g && m("deleted", b[G], G);
                                    break;
                                case "splice":
                                    c = Math.min(Math.max(0, 0 > d[0] ? g + d[0] : d[0]), g);
                                    for (var g = 1 === t ? g : Math.min(c + (d[1] || 0), g), t = c + t - 2, G = Math.max(g, t), P = [], n = [], Q = 2; c < G; ++c,
                                        ++Q) c < g && n.push(m("deleted", b[c], c)), c < t && P.push(m("added", d[Q], c));
                                    a.a.dc(n, P);
                                    break;
                                default:
                                    return
                            }
                            f = l
                        }
                    }
                }
            };
            var s = a.a.Yb("_state");
            a.m = a.B = function(b, c, d) {
                function e() {
                    if (0 < arguments.length) {
                        if ("function" === typeof f) f.apply(g.pb, arguments);
                        else throw Error("Cannot write a value to a ko.computed unless you specify a 'write' option. If you wish to read the current value, don't pass any parameters.");
                        return this
                    }
                    a.l.oc(e);
                    (g.S || g.s && e.Qa()) && e.aa();
                    return g.T
                }
                "object" === typeof b ? d = b : (d = d || {}, b && (d.read =
                    b));
                if ("function" != typeof d.read) throw Error("Pass a function that returns the value of the ko.computed");
                var f = d.write,
                    g = {
                        T: n,
                        S: !0,
                        Ra: !1,
                        Fb: !1,
                        R: !1,
                        Va: !1,
                        s: !1,
                        jd: d.read,
                        pb: c || d.owner,
                        i: d.disposeWhenNodeIsRemoved || d.i || null,
                        wa: d.disposeWhen || d.wa,
                        mb: null,
                        r: {},
                        L: 0,
                        bc: null
                    };
                e[s] = g;
                e.Vc = "function" === typeof f;
                a.a.ka || a.a.extend(e, a.J.fn);
                a.J.fn.rb(e);
                a.a.Ya(e, z);
                d.pure ? (g.Va = !0, g.s = !0, a.a.extend(e, $)) : d.deferEvaluation && a.a.extend(e, aa);
                a.options.deferUpdates && a.ya.deferred(e, !0);
                g.i && (g.Fb = !0, g.i.nodeType ||
                    (g.i = null));
                g.s || d.deferEvaluation || e.aa();
                g.i && e.ba() && a.a.F.oa(g.i, g.mb = function() {
                    e.k()
                });
                return e
            };
            var z = {
                    equalityComparer: J,
                    Aa: function() {
                        return this[s].L
                    },
                    Pb: function(a, c, d) {
                        if (this[s].Va && c === this) throw Error("A 'pure' computed must not be called recursively");
                        this[s].r[a] = d;
                        d.Ga = this[s].L++;
                        d.na = c.Na()
                    },
                    Qa: function() {
                        var a, c, d = this[s].r;
                        for (a in d)
                            if (d.hasOwnProperty(a) && (c = d[a], c.ia.Uc(c.na))) return !0
                    },
                    bd: function() {
                        this.Fa && !this[s].Ra && this.Fa()
                    },
                    ba: function() {
                        return this[s].S || 0 < this[s].L
                    },
                    ld: function() {
                        this.Mb || this.ac()
                    },
                    uc: function(a) {
                        if (a.cb && !this[s].i) {
                            var c = a.X(this.bd, this, "dirty"),
                                d = a.X(this.ld, this);
                            return {
                                ia: a,
                                k: function() {
                                    c.k();
                                    d.k()
                                }
                            }
                        }
                        return a.X(this.ac, this)
                    },
                    ac: function() {
                        var b = this,
                            c = b.throttleEvaluation;
                        c && 0 <= c ? (clearTimeout(this[s].bc), this[s].bc = a.a.setTimeout(function() {
                            b.aa(!0)
                        }, c)) : b.Fa ? b.Fa() : b.aa(!0)
                    },
                    aa: function(b) {
                        var c = this[s],
                            d = c.wa;
                        if (!c.Ra && !c.R) {
                            if (c.i && !a.a.nb(c.i) || d && d()) {
                                if (!c.Fb) {
                                    this.k();
                                    return
                                }
                            } else c.Fb = !1;
                            c.Ra = !0;
                            try {
                                this.Qc(b)
                            } finally {
                                c.Ra = !1
                            }
                            c.L ||
                                this.k()
                        }
                    },
                    Qc: function(b) {
                        var c = this[s],
                            d = c.Va ? n : !c.L,
                            e = {
                                Hc: this,
                                Ma: c.r,
                                lb: c.L
                            };
                        a.l.Ub({
                            Gc: e,
                            gb: Y,
                            m: this,
                            Sa: d
                        });
                        c.r = {};
                        c.L = 0;
                        e = this.Pc(c, e);
                        this.tb(c.T, e) && (c.s || this.notifySubscribers(c.T, "beforeChange"), c.T = e, c.s ? this.zc() : b && this.notifySubscribers(c.T));
                        d && this.notifySubscribers(c.T, "awake")
                    },
                    Pc: function(b, c) {
                        try {
                            var d = b.jd;
                            return b.pb ? d.call(b.pb) : d()
                        } finally {
                            a.l.end(), c.lb && !b.s && a.a.D(c.Ma, X), b.S = !1
                        }
                    },
                    t: function() {
                        var a = this[s];
                        (a.S && !a.L || a.s && this.Qa()) && this.aa();
                        return a.T
                    },
                    Ta: function(b) {
                        a.J.fn.Ta.call(this,
                            b);
                        this.Fa = function() {
                            this.Kb(this[s].T);
                            this[s].S = !0;
                            this.Lb(this)
                        }
                    },
                    k: function() {
                        var b = this[s];
                        !b.s && b.r && a.a.D(b.r, function(a, b) {
                            b.k && b.k()
                        });
                        b.i && b.mb && a.a.F.pc(b.i, b.mb);
                        b.r = null;
                        b.L = 0;
                        b.R = !0;
                        b.S = !1;
                        b.s = !1;
                        b.i = null
                    }
                },
                $ = {
                    sa: function(b) {
                        var c = this,
                            d = c[s];
                        if (!d.R && d.s && "change" == b) {
                            d.s = !1;
                            if (d.S || c.Qa()) d.r = null, d.L = 0, d.S = !0, c.aa();
                            else {
                                var e = [];
                                a.a.D(d.r, function(a, b) {
                                    e[b.Ga] = a
                                });
                                a.a.q(e, function(a, b) {
                                    var e = d.r[a],
                                        l = c.uc(e.ia);
                                    l.Ga = b;
                                    l.na = e.na;
                                    d.r[a] = l
                                })
                            }
                            d.R || c.notifySubscribers(d.T, "awake")
                        }
                    },
                    Ia: function(b) {
                        var c = this[s];
                        c.R || "change" != b || this.Pa("change") || (a.a.D(c.r, function(a, b) {
                            b.k && (c.r[a] = {
                                ia: b.ia,
                                Ga: b.Ga,
                                na: b.na
                            }, b.k())
                        }), c.s = !0, this.notifySubscribers(n, "asleep"))
                    },
                    Na: function() {
                        var b = this[s];
                        b.s && (b.S || this.Qa()) && this.aa();
                        return a.J.fn.Na.call(this)
                    }
                },
                aa = {
                    sa: function(a) {
                        "change" != a && "beforeChange" != a || this.t()
                    }
                };
            a.a.ka && a.a.Xa(z, a.J.fn);
            var R = a.N.gd;
            a.m[R] = a.N;
            z[R] = a.m;
            a.Xc = function(b) {
                return a.Oa(b, a.m)
            };
            a.Yc = function(b) {
                return a.Oa(b, a.m) && b[s] && b[s].Va
            };
            a.b("computed", a.m);
            a.b("dependentObservable", a.m);
            a.b("isComputed", a.Xc);
            a.b("isPureComputed", a.Yc);
            a.b("computed.fn", z);
            a.G(z, "peek", z.t);
            a.G(z, "dispose", z.k);
            a.G(z, "isActive", z.ba);
            a.G(z, "getDependenciesCount", z.Aa);
            a.nc = function(b, c) {
                if ("function" === typeof b) return a.m(b, c, {
                    pure: !0
                });
                b = a.a.extend({}, b);
                b.pure = !0;
                return a.m(b, c)
            };
            a.b("pureComputed", a.nc);
            (function() {
                function b(a, f, g) {
                    g = g || new d;
                    a = f(a);
                    if ("object" != typeof a || null === a || a === n || a instanceof RegExp || a instanceof Date || a instanceof String || a instanceof Number || a instanceof Boolean) return a;
                    var k = a instanceof Array ? [] : {};
                    g.save(a, k);
                    c(a, function(c) {
                        var d = f(a[c]);
                        switch (typeof d) {
                            case "boolean":
                            case "number":
                            case "string":
                            case "function":
                                k[c] = d;
                                break;
                            case "object":
                            case "undefined":
                                var h = g.get(d);
                                k[c] = h !== n ? h : b(d, f, g)
                        }
                    });
                    return k
                }

                function c(a, b) {
                    if (a instanceof Array) {
                        for (var c = 0; c < a.length; c++) b(c);
                        "function" == typeof a.toJSON && b("toJSON")
                    } else
                        for (c in a) b(c)
                }

                function d() {
                    this.keys = [];
                    this.Ib = []
                }
                a.wc = function(c) {
                    if (0 == arguments.length) throw Error("When calling ko.toJS, pass the object you want to convert.");
                    return b(c, function(b) {
                        for (var c = 0; a.H(b) && 10 > c; c++) b = b();
                        return b
                    })
                };
                a.toJSON = function(b, c, d) {
                    b = a.wc(b);
                    return a.a.Eb(b, c, d)
                };
                d.prototype = {
                    save: function(b, c) {
                        var d = a.a.o(this.keys, b);
                        0 <= d ? this.Ib[d] = c : (this.keys.push(b), this.Ib.push(c))
                    },
                    get: function(b) {
                        b = a.a.o(this.keys, b);
                        return 0 <= b ? this.Ib[b] : n
                    }
                }
            })();
            a.b("toJS", a.wc);
            a.b("toJSON", a.toJSON);
            (function() {
                a.j = {
                    u: function(b) {
                        switch (a.a.A(b)) {
                            case "option":
                                return !0 === b.__ko__hasDomDataOptionValue__ ? a.a.e.get(b, a.d.options.xb) : 7 >= a.a.C ? b.getAttributeNode("value") &&
                                    b.getAttributeNode("value").specified ? b.value : b.text : b.value;
                            case "select":
                                return 0 <= b.selectedIndex ? a.j.u(b.options[b.selectedIndex]) : n;
                            default:
                                return b.value
                        }
                    },
                    ha: function(b, c, d) {
                        switch (a.a.A(b)) {
                            case "option":
                                switch (typeof c) {
                                    case "string":
                                        a.a.e.set(b, a.d.options.xb, n);
                                        "__ko__hasDomDataOptionValue__" in b && delete b.__ko__hasDomDataOptionValue__;
                                        b.value = c;
                                        break;
                                    default:
                                        a.a.e.set(b, a.d.options.xb, c), b.__ko__hasDomDataOptionValue__ = !0, b.value = "number" === typeof c ? c : ""
                                }
                                break;
                            case "select":
                                if ("" === c ||
                                    null === c) c = n;
                                for (var e = -1, f = 0, g = b.options.length, k; f < g; ++f)
                                    if (k = a.j.u(b.options[f]), k == c || "" == k && c === n) {
                                        e = f;
                                        break
                                    } if (d || 0 <= e || c === n && 1 < b.size) b.selectedIndex = e;
                                break;
                            default:
                                if (null === c || c === n) c = "";
                                b.value = c
                        }
                    }
                }
            })();
            a.b("selectExtensions", a.j);
            a.b("selectExtensions.readValue", a.j.u);
            a.b("selectExtensions.writeValue", a.j.ha);
            a.h = function() {
                function b(b) {
                    b = a.a.$a(b);
                    123 === b.charCodeAt(0) && (b = b.slice(1, -1));
                    var c = [],
                        d = b.match(e),
                        r, k = [],
                        p = 0;
                    if (d) {
                        d.push(",");
                        for (var A = 0, y; y = d[A]; ++A) {
                            var t = y.charCodeAt(0);
                            if (44 === t) {
                                if (0 >= p) {
                                    c.push(r && k.length ? {
                                        key: r,
                                        value: k.join("")
                                    } : {
                                        unknown: r || k.join("")
                                    });
                                    r = p = 0;
                                    k = [];
                                    continue
                                }
                            } else if (58 === t) {
                                if (!p && !r && 1 === k.length) {
                                    r = k.pop();
                                    continue
                                }
                            } else 47 === t && A && 1 < y.length ? (t = d[A - 1].match(f)) && !g[t[0]] && (b = b.substr(b.indexOf(y) + 1), d = b.match(e), d.push(","), A = -1, y = "/") : 40 === t || 123 === t || 91 === t ? ++p : 41 === t || 125 === t || 93 === t ? --p : r || k.length || 34 !== t && 39 !== t || (y = y.slice(1, -1));
                            k.push(y)
                        }
                    }
                    return c
                }
                var c = ["true", "false", "null", "undefined"],
                    d = /^(?:[$_a-z][$\w]*|(.+)(\.\s*[$_a-z][$\w]*|\[.+\]))$/i,
                    e = RegExp("\"(?:[^\"\\\\]|\\\\.)*\"|'(?:[^'\\\\]|\\\\.)*'|/(?:[^/\\\\]|\\\\.)*/w*|[^\\s:,/][^,\"'{}()/:[\\]]*[^\\s,\"'{}()/:[\\]]|[^\\s]", "g"),
                    f = /[\])"'A-Za-z0-9_$]+$/,
                    g = {
                        "in": 1,
                        "return": 1,
                        "typeof": 1
                    },
                    k = {};
                return {
                    ta: [],
                    ea: k,
                    yb: b,
                    Ua: function(e, m) {
                        function h(b, e) {
                            var m;
                            if (!A) {
                                var l = a.getBindingHandler(b);
                                if (l && l.preprocess && !(e = l.preprocess(e, b, h))) return;
                                if (l = k[b]) m = e, 0 <= a.a.o(c, m) ? m = !1 : (l = m.match(d), m = null === l ? !1 : l[1] ? "Object(" + l[1] + ")" + l[2] : m), l = m;
                                l && g.push("'" + b + "':function(_z){" + m + "=_z}")
                            }
                            p && (e =
                                "function(){return " + e + " }");
                            f.push("'" + b + "':" + e)
                        }
                        m = m || {};
                        var f = [],
                            g = [],
                            p = m.valueAccessors,
                            A = m.bindingParams,
                            y = "string" === typeof e ? b(e) : e;
                        a.a.q(y, function(a) {
                            h(a.key || a.unknown, a.value)
                        });
                        g.length && h("_ko_property_writers", "{" + g.join(",") + " }");
                        return f.join(",")
                    },
                    ad: function(a, b) {
                        for (var c = 0; c < a.length; c++)
                            if (a[c].key == b) return !0;
                        return !1
                    },
                    Ea: function(b, c, d, e, f) {
                        if (b && a.H(b)) !a.Ba(b) || f && b.t() === e || b(e);
                        else if ((b = c.get("_ko_property_writers")) && b[d]) b[d](e)
                    }
                }
            }();
            a.b("expressionRewriting", a.h);
            a.b("expressionRewriting.bindingRewriteValidators",
                a.h.ta);
            a.b("expressionRewriting.parseObjectLiteral", a.h.yb);
            a.b("expressionRewriting.preProcessBindings", a.h.Ua);
            a.b("expressionRewriting._twoWayBindings", a.h.ea);
            a.b("jsonExpressionRewriting", a.h);
            a.b("jsonExpressionRewriting.insertPropertyAccessorsIntoJson", a.h.Ua);
            (function() {
                function b(a) {
                    return 8 == a.nodeType && g.test(f ? a.text : a.nodeValue)
                }

                function c(a) {
                    return 8 == a.nodeType && k.test(f ? a.text : a.nodeValue)
                }

                function d(a, d) {
                    for (var e = a, f = 1, l = []; e = e.nextSibling;) {
                        if (c(e) && (f--, 0 === f)) return l;
                        l.push(e);
                        b(e) && f++
                    }
                    if (!d) throw Error("Cannot find closing comment tag to match: " + a.nodeValue);
                    return null
                }

                function e(a, b) {
                    var c = d(a, b);
                    return c ? 0 < c.length ? c[c.length - 1].nextSibling : a.nextSibling : null
                }
                var f = u && "\x3c!--test--\x3e" === u.createComment("test").text,
                    g = f ? /^\x3c!--\s*ko(?:\s+([\s\S]+))?\s*--\x3e$/ : /^\s*ko(?:\s+([\s\S]+))?\s*$/,
                    k = f ? /^\x3c!--\s*\/ko\s*--\x3e$/ : /^\s*\/ko\s*$/,
                    l = {
                        ul: !0,
                        ol: !0
                    };
                a.f = {
                    Z: {},
                    childNodes: function(a) {
                        return b(a) ? d(a) : a.childNodes
                    },
                    xa: function(c) {
                        if (b(c)) {
                            c = a.f.childNodes(c);
                            for (var d =
                                    0, e = c.length; d < e; d++) a.removeNode(c[d])
                        } else a.a.ob(c)
                    },
                    da: function(c, d) {
                        if (b(c)) {
                            a.f.xa(c);
                            for (var e = c.nextSibling, f = 0, l = d.length; f < l; f++) e.parentNode.insertBefore(d[f], e)
                        } else a.a.da(c, d)
                    },
                    mc: function(a, c) {
                        b(a) ? a.parentNode.insertBefore(c, a.nextSibling) : a.firstChild ? a.insertBefore(c, a.firstChild) : a.appendChild(c)
                    },
                    gc: function(c, d, e) {
                        e ? b(c) ? c.parentNode.insertBefore(d, e.nextSibling) : e.nextSibling ? c.insertBefore(d, e.nextSibling) : c.appendChild(d) : a.f.mc(c, d)
                    },
                    firstChild: function(a) {
                        return b(a) ? !a.nextSibling ||
                            c(a.nextSibling) ? null : a.nextSibling : a.firstChild
                    },
                    nextSibling: function(a) {
                        b(a) && (a = e(a));
                        return a.nextSibling && c(a.nextSibling) ? null : a.nextSibling
                    },
                    Tc: b,
                    pd: function(a) {
                        return (a = (f ? a.text : a.nodeValue).match(g)) ? a[1] : null
                    },
                    kc: function(d) {
                        if (l[a.a.A(d)]) {
                            var h = d.firstChild;
                            if (h) {
                                do
                                    if (1 === h.nodeType) {
                                        var f;
                                        f = h.firstChild;
                                        var g = null;
                                        if (f) {
                                            do
                                                if (g) g.push(f);
                                                else if (b(f)) {
                                                var k = e(f, !0);
                                                k ? f = k : g = [f]
                                            } else c(f) && (g = [f]);
                                            while (f = f.nextSibling)
                                        }
                                        if (f = g)
                                            for (g = h.nextSibling, k = 0; k < f.length; k++) g ? d.insertBefore(f[k],
                                                g) : d.appendChild(f[k])
                                    } while (h = h.nextSibling)
                            }
                        }
                    }
                }
            })();
            a.b("virtualElements", a.f);
            a.b("virtualElements.allowedBindings", a.f.Z);
            a.b("virtualElements.emptyNode", a.f.xa);
            a.b("virtualElements.insertAfter", a.f.gc);
            a.b("virtualElements.prepend", a.f.mc);
            a.b("virtualElements.setDomNodeChildren", a.f.da);
            (function() {
                a.Q = function() {
                    this.Fc = {}
                };
                a.a.extend(a.Q.prototype, {
                    nodeHasBindings: function(b) {
                        switch (b.nodeType) {
                            case 1:
                                return null != b.getAttribute("data-bind") || a.g.getComponentNameForNode(b);
                            case 8:
                                return a.f.Tc(b);
                            default:
                                return !1
                        }
                    },
                    getBindings: function(b, c) {
                        var d = this.getBindingsString(b, c),
                            d = d ? this.parseBindingsString(d, c, b) : null;
                        return a.g.Ob(d, b, c, !1)
                    },
                    getBindingAccessors: function(b, c) {
                        var d = this.getBindingsString(b, c),
                            d = d ? this.parseBindingsString(d, c, b, {
                                valueAccessors: !0
                            }) : null;
                        return a.g.Ob(d, b, c, !0)
                    },
                    getBindingsString: function(b) {
                        switch (b.nodeType) {
                            case 1:
                                return b.getAttribute("data-bind");
                            case 8:
                                return a.f.pd(b);
                            default:
                                return null
                        }
                    },
                    parseBindingsString: function(b, c, d, e) {
                        try {
                            var f = this.Fc,
                                g = b + (e && e.valueAccessors ||
                                    ""),
                                k;
                            if (!(k = f[g])) {
                                var l, m = "with($context){with($data||{}){return{" + a.h.Ua(b, e) + "}}}";
                                l = new Function("$context", "$element", m);
                                k = f[g] = l
                            }
                            return k(c, d)
                        } catch (h) {
                            throw h.message = "Unable to parse bindings.\nBindings value: " + b + "\nMessage: " + h.message, h;
                        }
                    }
                });
                a.Q.instance = new a.Q
            })();
            a.b("bindingProvider", a.Q);
            (function() {
                function b(a) {
                    return function() {
                        return a
                    }
                }

                function c(a) {
                    return a()
                }

                function d(b) {
                    return a.a.Ca(a.l.w(b), function(a, c) {
                        return function() {
                            return b()[c]
                        }
                    })
                }

                function e(c, e, h) {
                    return "function" ===
                        typeof c ? d(c.bind(null, e, h)) : a.a.Ca(c, b)
                }

                function f(a, b) {
                    return d(this.getBindings.bind(this, a, b))
                }

                function g(b, c, d) {
                    var e, h = a.f.firstChild(c),
                        f = a.Q.instance,
                        m = f.preprocessNode;
                    if (m) {
                        for (; e = h;) h = a.f.nextSibling(e), m.call(f, e);
                        h = a.f.firstChild(c)
                    }
                    for (; e = h;) h = a.f.nextSibling(e), k(b, e, d)
                }

                function k(b, c, d) {
                    var e = !0,
                        h = 1 === c.nodeType;
                    h && a.f.kc(c);
                    if (h && d || a.Q.instance.nodeHasBindings(c)) e = m(c, null, b, d).shouldBindDescendants;
                    e && !r[a.a.A(c)] && g(b, c, !h)
                }

                function l(b) {
                    var c = [],
                        d = {},
                        e = [];
                    a.a.D(b, function Z(h) {
                        if (!d[h]) {
                            var f =
                                a.getBindingHandler(h);
                            f && (f.after && (e.push(h), a.a.q(f.after, function(c) {
                                if (b[c]) {
                                    if (-1 !== a.a.o(e, c)) throw Error("Cannot combine the following bindings, because they have a cyclic dependency: " + e.join(", "));
                                    Z(c)
                                }
                            }), e.length--), c.push({
                                key: h,
                                fc: f
                            }));
                            d[h] = !0
                        }
                    });
                    return c
                }

                function m(b, d, e, h) {
                    var m = a.a.e.get(b, q);
                    if (!d) {
                        if (m) throw Error("You cannot apply bindings multiple times to the same element.");
                        a.a.e.set(b, q, !0)
                    }!m && h && a.tc(b, e);
                    var g;
                    if (d && "function" !== typeof d) g = d;
                    else {
                        var k = a.Q.instance,
                            r = k.getBindingAccessors ||
                            f,
                            p = a.B(function() {
                                (g = d ? d(e, b) : r.call(k, b, e)) && e.P && e.P();
                                return g
                            }, null, {
                                i: b
                            });
                        g && p.ba() || (p = null)
                    }
                    var u;
                    if (g) {
                        var v = p ? function(a) {
                                return function() {
                                    return c(p()[a])
                                }
                            } : function(a) {
                                return g[a]
                            },
                            s = function() {
                                return a.a.Ca(p ? p() : g, c)
                            };
                        s.get = function(a) {
                            return g[a] && c(v(a))
                        };
                        s.has = function(a) {
                            return a in g
                        };
                        h = l(g);
                        a.a.q(h, function(c) {
                            var d = c.fc.init,
                                h = c.fc.update,
                                f = c.key;
                            if (8 === b.nodeType && !a.f.Z[f]) throw Error("The binding '" + f + "' cannot be used with virtual elements");
                            try {
                                "function" == typeof d && a.l.w(function() {
                                    var a =
                                        d(b, v(f), s, e.$data, e);
                                    if (a && a.controlsDescendantBindings) {
                                        if (u !== n) throw Error("Multiple bindings (" + u + " and " + f + ") are trying to control descendant bindings of the same element. You cannot use these bindings together on the same element.");
                                        u = f
                                    }
                                }), "function" == typeof h && a.B(function() {
                                    h(b, v(f), s, e.$data, e)
                                }, null, {
                                    i: b
                                })
                            } catch (m) {
                                throw m.message = 'Unable to process binding "' + f + ": " + g[f] + '"\nMessage: ' + m.message, m;
                            }
                        })
                    }
                    return {
                        shouldBindDescendants: u === n
                    }
                }

                function h(b) {
                    return b && b instanceof a.U ? b : new a.U(b)
                }
                a.d = {};
                var r = {
                    script: !0,
                    textarea: !0,
                    template: !0
                };
                a.getBindingHandler = function(b) {
                    return a.d[b]
                };
                a.U = function(b, c, d, e) {
                    var h = this,
                        f = "function" == typeof b && !a.H(b),
                        m, g = a.B(function() {
                            var m = f ? b() : b,
                                l = a.a.c(m);
                            c ? (c.P && c.P(), a.a.extend(h, c), g && (h.P = g)) : (h.$parents = [], h.$root = l, h.ko = a);
                            h.$rawData = m;
                            h.$data = l;
                            d && (h[d] = l);
                            e && e(h, c, l);
                            return h.$data
                        }, null, {
                            wa: function() {
                                return m && !a.a.Qb(m)
                            },
                            i: !0
                        });
                    g.ba() && (h.P = g, g.equalityComparer = null, m = [], g.Ac = function(b) {
                        m.push(b);
                        a.a.F.oa(b, function(b) {
                            a.a.La(m, b);
                            m.length ||
                                (g.k(), h.P = g = n)
                        })
                    })
                };
                a.U.prototype.createChildContext = function(b, c, d) {
                    return new a.U(b, this, c, function(a, b) {
                        a.$parentContext = b;
                        a.$parent = b.$data;
                        a.$parents = (b.$parents || []).slice(0);
                        a.$parents.unshift(a.$parent);
                        d && d(a)
                    })
                };
                a.U.prototype.extend = function(b) {
                    return new a.U(this.P || this.$data, this, null, function(c, d) {
                        c.$rawData = d.$rawData;
                        a.a.extend(c, "function" == typeof b ? b() : b)
                    })
                };
                var q = a.a.e.I(),
                    p = a.a.e.I();
                a.tc = function(b, c) {
                    if (2 == arguments.length) a.a.e.set(b, p, c), c.P && c.P.Ac(b);
                    else return a.a.e.get(b,
                        p)
                };
                a.Ja = function(b, c, d) {
                    1 === b.nodeType && a.f.kc(b);
                    return m(b, c, h(d), !0)
                };
                a.Dc = function(b, c, d) {
                    d = h(d);
                    return a.Ja(b, e(c, d, b), d)
                };
                a.eb = function(a, b) {
                    1 !== b.nodeType && 8 !== b.nodeType || g(h(a), b, !0)
                };
                a.Rb = function(a, b) {
                    !v && x.jQuery && (v = x.jQuery);
                    if (b && 1 !== b.nodeType && 8 !== b.nodeType) throw Error("ko.applyBindings: first parameter should be your view model; second parameter should be a DOM node");
                    b = b || x.document.body;
                    k(h(a), b, !0)
                };
                a.kb = function(b) {
                    switch (b.nodeType) {
                        case 1:
                        case 8:
                            var c = a.tc(b);
                            if (c) return c;
                            if (b.parentNode) return a.kb(b.parentNode)
                    }
                    return n
                };
                a.Jc = function(b) {
                    return (b = a.kb(b)) ? b.$data : n
                };
                a.b("bindingHandlers", a.d);
                a.b("applyBindings", a.Rb);
                a.b("applyBindingsToDescendants", a.eb);
                a.b("applyBindingAccessorsToNode", a.Ja);
                a.b("applyBindingsToNode", a.Dc);
                a.b("contextFor", a.kb);
                a.b("dataFor", a.Jc)
            })();
            (function(b) {
                function c(c, e) {
                    var m = f.hasOwnProperty(c) ? f[c] : b,
                        h;
                    m ? m.X(e) : (m = f[c] = new a.J, m.X(e), d(c, function(b, d) {
                        var e = !(!d || !d.synchronous);
                        g[c] = {
                            definition: b,
                            Zc: e
                        };
                        delete f[c];
                        h || e ? m.notifySubscribers(b) :
                            a.Y.Wa(function() {
                                m.notifySubscribers(b)
                            })
                    }), h = !0)
                }

                function d(a, b) {
                    e("getConfig", [a], function(c) {
                        c ? e("loadComponent", [a, c], function(a) {
                            b(a, c)
                        }) : b(null, null)
                    })
                }

                function e(c, d, f, h) {
                    h || (h = a.g.loaders.slice(0));
                    var g = h.shift();
                    if (g) {
                        var q = g[c];
                        if (q) {
                            var p = !1;
                            if (q.apply(g, d.concat(function(a) {
                                    p ? f(null) : null !== a ? f(a) : e(c, d, f, h)
                                })) !== b && (p = !0, !g.suppressLoaderExceptions)) throw Error("Component loaders must supply values by invoking the callback, not by returning values synchronously.");
                        } else e(c, d, f, h)
                    } else f(null)
                }
                var f = {},
                    g = {};
                a.g = {
                    get: function(d, e) {
                        var f = g.hasOwnProperty(d) ? g[d] : b;
                        f ? f.Zc ? a.l.w(function() {
                            e(f.definition)
                        }) : a.Y.Wa(function() {
                            e(f.definition)
                        }) : c(d, e)
                    },
                    Xb: function(a) {
                        delete g[a]
                    },
                    Jb: e
                };
                a.g.loaders = [];
                a.b("components", a.g);
                a.b("components.get", a.g.get);
                a.b("components.clearCachedDefinition", a.g.Xb)
            })();
            (function() {
                function b(b, c, d, e) {
                    function g() {
                        0 === --y && e(k)
                    }
                    var k = {},
                        y = 2,
                        t = d.template;
                    d = d.viewModel;
                    t ? f(c, t, function(c) {
                        a.g.Jb("loadTemplate", [b, c], function(a) {
                            k.template = a;
                            g()
                        })
                    }) : g();
                    d ? f(c, d, function(c) {
                        a.g.Jb("loadViewModel",
                            [b, c],
                            function(a) {
                                k[l] = a;
                                g()
                            })
                    }) : g()
                }

                function c(a, b, d) {
                    if ("function" === typeof b) d(function(a) {
                        return new b(a)
                    });
                    else if ("function" === typeof b[l]) d(b[l]);
                    else if ("instance" in b) {
                        var e = b.instance;
                        d(function() {
                            return e
                        })
                    } else "viewModel" in b ? c(a, b.viewModel, d) : a("Unknown viewModel value: " + b)
                }

                function d(b) {
                    switch (a.a.A(b)) {
                        case "script":
                            return a.a.ma(b.text);
                        case "textarea":
                            return a.a.ma(b.value);
                        case "template":
                            if (e(b.content)) return a.a.ua(b.content.childNodes)
                    }
                    return a.a.ua(b.childNodes)
                }

                function e(a) {
                    return x.DocumentFragment ?
                        a instanceof DocumentFragment : a && 11 === a.nodeType
                }

                function f(a, b, c) {
                    "string" === typeof b.require ? O || x.require ? (O || x.require)([b.require], c) : a("Uses require, but no AMD loader is present") : c(b)
                }

                function g(a) {
                    return function(b) {
                        throw Error("Component '" + a + "': " + b);
                    }
                }
                var k = {};
                a.g.register = function(b, c) {
                    if (!c) throw Error("Invalid configuration for " + b);
                    if (a.g.ub(b)) throw Error("Component " + b + " is already registered");
                    k[b] = c
                };
                a.g.ub = function(a) {
                    return k.hasOwnProperty(a)
                };
                a.g.od = function(b) {
                    delete k[b];
                    a.g.Xb(b)
                };
                a.g.Zb = {
                    getConfig: function(a, b) {
                        b(k.hasOwnProperty(a) ? k[a] : null)
                    },
                    loadComponent: function(a, c, d) {
                        var e = g(a);
                        f(e, c, function(c) {
                            b(a, e, c, d)
                        })
                    },
                    loadTemplate: function(b, c, f) {
                        b = g(b);
                        if ("string" === typeof c) f(a.a.ma(c));
                        else if (c instanceof Array) f(c);
                        else if (e(c)) f(a.a.V(c.childNodes));
                        else if (c.element)
                            if (c = c.element, x.HTMLElement ? c instanceof HTMLElement : c && c.tagName && 1 === c.nodeType) f(d(c));
                            else if ("string" === typeof c) {
                            var l = u.getElementById(c);
                            l ? f(d(l)) : b("Cannot find element with ID " + c)
                        } else b("Unknown element type: " +
                            c);
                        else b("Unknown template value: " + c)
                    },
                    loadViewModel: function(a, b, d) {
                        c(g(a), b, d)
                    }
                };
                var l = "createViewModel";
                a.b("components.register", a.g.register);
                a.b("components.isRegistered", a.g.ub);
                a.b("components.unregister", a.g.od);
                a.b("components.defaultLoader", a.g.Zb);
                a.g.loaders.push(a.g.Zb);
                a.g.Bc = k
            })();
            (function() {
                function b(b, e) {
                    var f = b.getAttribute("params");
                    if (f) {
                        var f = c.parseBindingsString(f, e, b, {
                                valueAccessors: !0,
                                bindingParams: !0
                            }),
                            f = a.a.Ca(f, function(c) {
                                return a.m(c, null, {
                                    i: b
                                })
                            }),
                            g = a.a.Ca(f, function(c) {
                                var e =
                                    c.t();
                                return c.ba() ? a.m({
                                    read: function() {
                                        return a.a.c(c())
                                    },
                                    write: a.Ba(e) && function(a) {
                                        c()(a)
                                    },
                                    i: b
                                }) : e
                            });
                        g.hasOwnProperty("$raw") || (g.$raw = f);
                        return g
                    }
                    return {
                        $raw: {}
                    }
                }
                a.g.getComponentNameForNode = function(b) {
                    var c = a.a.A(b);
                    if (a.g.ub(c) && (-1 != c.indexOf("-") || "[object HTMLUnknownElement]" == "" + b || 8 >= a.a.C && b.tagName === c)) return c
                };
                a.g.Ob = function(c, e, f, g) {
                    if (1 === e.nodeType) {
                        var k = a.g.getComponentNameForNode(e);
                        if (k) {
                            c = c || {};
                            if (c.component) throw Error('Cannot use the "component" binding on a custom element matching a component');
                            var l = {
                                name: k,
                                params: b(e, f)
                            };
                            c.component = g ? function() {
                                return l
                            } : l
                        }
                    }
                    return c
                };
                var c = new a.Q;
                9 > a.a.C && (a.g.register = function(a) {
                    return function(b) {
                        u.createElement(b);
                        return a.apply(this, arguments)
                    }
                }(a.g.register), u.createDocumentFragment = function(b) {
                    return function() {
                        var c = b(),
                            f = a.g.Bc,
                            g;
                        for (g in f) f.hasOwnProperty(g) && c.createElement(g);
                        return c
                    }
                }(u.createDocumentFragment))
            })();
            (function(b) {
                function c(b, c, d) {
                    c = c.template;
                    if (!c) throw Error("Component '" + b + "' has no template");
                    b = a.a.ua(c);
                    a.f.da(d, b)
                }

                function d(a, b, c, d) {
                    var e = a.createViewModel;
                    return e ? e.call(a, d, {
                        element: b,
                        templateNodes: c
                    }) : d
                }
                var e = 0;
                a.d.component = {
                    init: function(f, g, k, l, m) {
                        function h() {
                            var a = r && r.dispose;
                            "function" === typeof a && a.call(r);
                            q = r = null
                        }
                        var r, q, p = a.a.V(a.f.childNodes(f));
                        a.a.F.oa(f, h);
                        a.m(function() {
                            var l = a.a.c(g()),
                                k, t;
                            "string" === typeof l ? k = l : (k = a.a.c(l.name), t = a.a.c(l.params));
                            if (!k) throw Error("No component name specified");
                            var n = q = ++e;
                            a.g.get(k, function(e) {
                                if (q === n) {
                                    h();
                                    if (!e) throw Error("Unknown component '" + k +
                                        "'");
                                    c(k, e, f);
                                    var g = d(e, f, p, t);
                                    e = m.createChildContext(g, b, function(a) {
                                        a.$component = g;
                                        a.$componentTemplateNodes = p
                                    });
                                    r = g;
                                    a.eb(e, f)
                                }
                            })
                        }, null, {
                            i: f
                        });
                        return {
                            controlsDescendantBindings: !0
                        }
                    }
                };
                a.f.Z.component = !0
            })();
            var S = {
                "class": "className",
                "for": "htmlFor"
            };
            a.d.attr = {
                update: function(b, c) {
                    var d = a.a.c(c()) || {};
                    a.a.D(d, function(c, d) {
                        d = a.a.c(d);
                        var g = !1 === d || null === d || d === n;
                        g && b.removeAttribute(c);
                        8 >= a.a.C && c in S ? (c = S[c], g ? b.removeAttribute(c) : b[c] = d) : g || b.setAttribute(c, d.toString());
                        "name" === c && a.a.rc(b,
                            g ? "" : d.toString())
                    })
                }
            };
            (function() {
                a.d.checked = {
                    after: ["value", "attr"],
                    init: function(b, c, d) {
                        function e() {
                            var e = b.checked,
                                f = p ? g() : e;
                            if (!a.va.Sa() && (!l || e)) {
                                var m = a.l.w(c);
                                if (h) {
                                    var k = r ? m.t() : m;
                                    q !== f ? (e && (a.a.pa(k, f, !0), a.a.pa(k, q, !1)), q = f) : a.a.pa(k, f, e);
                                    r && a.Ba(m) && m(k)
                                } else a.h.Ea(m, d, "checked", f, !0)
                            }
                        }

                        function f() {
                            var d = a.a.c(c());
                            b.checked = h ? 0 <= a.a.o(d, g()) : k ? d : g() === d
                        }
                        var g = a.nc(function() {
                                return d.has("checkedValue") ? a.a.c(d.get("checkedValue")) : d.has("value") ? a.a.c(d.get("value")) : b.value
                            }),
                            k =
                            "checkbox" == b.type,
                            l = "radio" == b.type;
                        if (k || l) {
                            var m = c(),
                                h = k && a.a.c(m) instanceof Array,
                                r = !(h && m.push && m.splice),
                                q = h ? g() : n,
                                p = l || h;
                            l && !b.name && a.d.uniqueName.init(b, function() {
                                return !0
                            });
                            a.m(e, null, {
                                i: b
                            });
                            a.a.p(b, "click", e);
                            a.m(f, null, {
                                i: b
                            });
                            m = n
                        }
                    }
                };
                a.h.ea.checked = !0;
                a.d.checkedValue = {
                    update: function(b, c) {
                        b.value = a.a.c(c())
                    }
                }
            })();
            a.d.css = {
                update: function(b, c) {
                    var d = a.a.c(c());
                    null !== d && "object" == typeof d ? a.a.D(d, function(c, d) {
                        d = a.a.c(d);
                        a.a.bb(b, c, d)
                    }) : (d = a.a.$a(String(d || "")), a.a.bb(b, b.__ko__cssValue,
                        !1), b.__ko__cssValue = d, a.a.bb(b, d, !0))
                }
            };
            a.d.enable = {
                update: function(b, c) {
                    var d = a.a.c(c());
                    d && b.disabled ? b.removeAttribute("disabled") : d || b.disabled || (b.disabled = !0)
                }
            };
            a.d.disable = {
                update: function(b, c) {
                    a.d.enable.update(b, function() {
                        return !a.a.c(c())
                    })
                }
            };
            a.d.event = {
                init: function(b, c, d, e, f) {
                    var g = c() || {};
                    a.a.D(g, function(g) {
                        "string" == typeof g && a.a.p(b, g, function(b) {
                            var m, h = c()[g];
                            if (h) {
                                try {
                                    var r = a.a.V(arguments);
                                    e = f.$data;
                                    r.unshift(e);
                                    m = h.apply(e, r)
                                } finally {
                                    !0 !== m && (b.preventDefault ? b.preventDefault() :
                                        b.returnValue = !1)
                                }!1 === d.get(g + "Bubble") && (b.cancelBubble = !0, b.stopPropagation && b.stopPropagation())
                            }
                        })
                    })
                }
            };
            a.d.foreach = {
                ic: function(b) {
                    return function() {
                        var c = b(),
                            d = a.a.zb(c);
                        if (!d || "number" == typeof d.length) return {
                            foreach: c,
                            templateEngine: a.W.sb
                        };
                        a.a.c(c);
                        return {
                            foreach: d.data,
                            as: d.as,
                            includeDestroyed: d.includeDestroyed,
                            afterAdd: d.afterAdd,
                            beforeRemove: d.beforeRemove,
                            afterRender: d.afterRender,
                            beforeMove: d.beforeMove,
                            afterMove: d.afterMove,
                            templateEngine: a.W.sb
                        }
                    }
                },
                init: function(b, c) {
                    return a.d.template.init(b,
                        a.d.foreach.ic(c))
                },
                update: function(b, c, d, e, f) {
                    return a.d.template.update(b, a.d.foreach.ic(c), d, e, f)
                }
            };
            a.h.ta.foreach = !1;
            a.f.Z.foreach = !0;
            a.d.hasfocus = {
                init: function(b, c, d) {
                    function e(e) {
                        b.__ko_hasfocusUpdating = !0;
                        var f = b.ownerDocument;
                        if ("activeElement" in f) {
                            var g;
                            try {
                                g = f.activeElement
                            } catch (h) {
                                g = f.body
                            }
                            e = g === b
                        }
                        f = c();
                        a.h.Ea(f, d, "hasfocus", e, !0);
                        b.__ko_hasfocusLastValue = e;
                        b.__ko_hasfocusUpdating = !1
                    }
                    var f = e.bind(null, !0),
                        g = e.bind(null, !1);
                    a.a.p(b, "focus", f);
                    a.a.p(b, "focusin", f);
                    a.a.p(b, "blur", g);
                    a.a.p(b,
                        "focusout", g)
                },
                update: function(b, c) {
                    var d = !!a.a.c(c());
                    b.__ko_hasfocusUpdating || b.__ko_hasfocusLastValue === d || (d ? b.focus() : b.blur(), !d && b.__ko_hasfocusLastValue && b.ownerDocument.body.focus(), a.l.w(a.a.Da, null, [b, d ? "focusin" : "focusout"]))
                }
            };
            a.h.ea.hasfocus = !0;
            a.d.hasFocus = a.d.hasfocus;
            a.h.ea.hasFocus = !0;
            a.d.html = {
                init: function() {
                    return {
                        controlsDescendantBindings: !0
                    }
                },
                update: function(b, c) {
                    a.a.Cb(b, c())
                }
            };
            K("if");
            K("ifnot", !1, !0);
            K("with", !0, !1, function(a, c) {
                return a.createChildContext(c)
            });
            var L = {};
            a.d.options = {
                init: function(b) {
                    if ("select" !== a.a.A(b)) throw Error("options binding applies only to SELECT elements");
                    for (; 0 < b.length;) b.remove(0);
                    return {
                        controlsDescendantBindings: !0
                    }
                },
                update: function(b, c, d) {
                    function e() {
                        return a.a.Ka(b.options, function(a) {
                            return a.selected
                        })
                    }

                    function f(a, b, c) {
                        var d = typeof b;
                        return "function" == d ? b(a) : "string" == d ? a[b] : c
                    }

                    function g(c, e) {
                        if (A && h) a.j.ha(b, a.a.c(d.get("value")), !0);
                        else if (p.length) {
                            var f = 0 <= a.a.o(p, a.j.u(e[0]));
                            a.a.sc(e[0], f);
                            A && !f && a.l.w(a.a.Da, null, [b,
                                "change"
                            ])
                        }
                    }
                    var k = b.multiple,
                        l = 0 != b.length && k ? b.scrollTop : null,
                        m = a.a.c(c()),
                        h = d.get("valueAllowUnset") && d.has("value"),
                        r = d.get("optionsIncludeDestroyed");
                    c = {};
                    var q, p = [];
                    h || (k ? p = a.a.fb(e(), a.j.u) : 0 <= b.selectedIndex && p.push(a.j.u(b.options[b.selectedIndex])));
                    m && ("undefined" == typeof m.length && (m = [m]), q = a.a.Ka(m, function(b) {
                        return r || b === n || null === b || !a.a.c(b._destroy)
                    }), d.has("optionsCaption") && (m = a.a.c(d.get("optionsCaption")), null !== m && m !== n && q.unshift(L)));
                    var A = !1;
                    c.beforeRemove = function(a) {
                        b.removeChild(a)
                    };
                    m = g;
                    d.has("optionsAfterRender") && "function" == typeof d.get("optionsAfterRender") && (m = function(b, c) {
                        g(0, c);
                        a.l.w(d.get("optionsAfterRender"), null, [c[0], b !== L ? b : n])
                    });
                    a.a.Bb(b, q, function(c, e, g) {
                        g.length && (p = !h && g[0].selected ? [a.j.u(g[0])] : [], A = !0);
                        e = b.ownerDocument.createElement("option");
                        c === L ? (a.a.Za(e, d.get("optionsCaption")), a.j.ha(e, n)) : (g = f(c, d.get("optionsValue"), c), a.j.ha(e, a.a.c(g)), c = f(c, d.get("optionsText"), g), a.a.Za(e, c));
                        return [e]
                    }, c, m);
                    a.l.w(function() {
                        h ? a.j.ha(b, a.a.c(d.get("value")),
                            !0) : (k ? p.length && e().length < p.length : p.length && 0 <= b.selectedIndex ? a.j.u(b.options[b.selectedIndex]) !== p[0] : p.length || 0 <= b.selectedIndex) && a.a.Da(b, "change")
                    });
                    a.a.Nc(b);
                    l && 20 < Math.abs(l - b.scrollTop) && (b.scrollTop = l)
                }
            };
            a.d.options.xb = a.a.e.I();
            a.d.selectedOptions = {
                after: ["options", "foreach"],
                init: function(b, c, d) {
                    a.a.p(b, "change", function() {
                        var e = c(),
                            f = [];
                        a.a.q(b.getElementsByTagName("option"), function(b) {
                            b.selected && f.push(a.j.u(b))
                        });
                        a.h.Ea(e, d, "selectedOptions", f)
                    })
                },
                update: function(b, c) {
                    if ("select" !=
                        a.a.A(b)) throw Error("values binding applies only to SELECT elements");
                    var d = a.a.c(c()),
                        e = b.scrollTop;
                    d && "number" == typeof d.length && a.a.q(b.getElementsByTagName("option"), function(b) {
                        var c = 0 <= a.a.o(d, a.j.u(b));
                        b.selected != c && a.a.sc(b, c)
                    });
                    b.scrollTop = e
                }
            };
            a.h.ea.selectedOptions = !0;
            a.d.style = {
                update: function(b, c) {
                    var d = a.a.c(c() || {});
                    a.a.D(d, function(c, d) {
                        d = a.a.c(d);
                        if (null === d || d === n || !1 === d) d = "";
                        b.style[c] = d
                    })
                }
            };
            a.d.submit = {
                init: function(b, c, d, e, f) {
                    if ("function" != typeof c()) throw Error("The value for a submit binding must be a function");
                    a.a.p(b, "submit", function(a) {
                        var d, e = c();
                        try {
                            d = e.call(f.$data, b)
                        } finally {
                            !0 !== d && (a.preventDefault ? a.preventDefault() : a.returnValue = !1)
                        }
                    })
                }
            };
            a.d.text = {
                init: function() {
                    return {
                        controlsDescendantBindings: !0
                    }
                },
                update: function(b, c) {
                    a.a.Za(b, c())
                }
            };
            a.f.Z.text = !0;
            (function() {
                if (x && x.navigator) var b = function(a) {
                        if (a) return parseFloat(a[1])
                    },
                    c = x.opera && x.opera.version && parseInt(x.opera.version()),
                    d = x.navigator.userAgent,
                    e = b(d.match(/^(?:(?!chrome).)*version\/([^ ]*) safari/i)),
                    f = b(d.match(/Firefox\/([^ ]*)/));
                if (10 > a.a.C) var g = a.a.e.I(),
                    k = a.a.e.I(),
                    l = function(b) {
                        var c = this.activeElement;
                        (c = c && a.a.e.get(c, k)) && c(b)
                    },
                    m = function(b, c) {
                        var d = b.ownerDocument;
                        a.a.e.get(d, g) || (a.a.e.set(d, g, !0), a.a.p(d, "selectionchange", l));
                        a.a.e.set(b, k, c)
                    };
                a.d.textInput = {
                    init: function(b, d, g) {
                        function l(c, d) {
                            a.a.p(b, c, d)
                        }

                        function k() {
                            var c = a.a.c(d());
                            if (null === c || c === n) c = "";
                            v !== n && c === v ? a.a.setTimeout(k, 4) : b.value !== c && (u = c, b.value = c)
                        }

                        function y() {
                            s || (v = b.value, s = a.a.setTimeout(t, 4))
                        }

                        function t() {
                            clearTimeout(s);
                            v = s = n;
                            var c =
                                b.value;
                            u !== c && (u = c, a.h.Ea(d(), g, "textInput", c))
                        }
                        var u = b.value,
                            s, v, x = 9 == a.a.C ? y : t;
                        10 > a.a.C ? (l("propertychange", function(a) {
                            "value" === a.propertyName && x(a)
                        }), 8 == a.a.C && (l("keyup", t), l("keydown", t)), 8 <= a.a.C && (m(b, x), l("dragend", y))) : (l("input", t), 5 > e && "textarea" === a.a.A(b) ? (l("keydown", y), l("paste", y), l("cut", y)) : 11 > c ? l("keydown", y) : 4 > f && (l("DOMAutoComplete", t), l("dragdrop", t), l("drop", t)));
                        l("change", t);
                        a.m(k, null, {
                            i: b
                        })
                    }
                };
                a.h.ea.textInput = !0;
                a.d.textinput = {
                    preprocess: function(a, b, c) {
                        c("textInput",
                            a)
                    }
                }
            })();
            a.d.uniqueName = {
                init: function(b, c) {
                    if (c()) {
                        var d = "ko_unique_" + ++a.d.uniqueName.Ic;
                        a.a.rc(b, d)
                    }
                }
            };
            a.d.uniqueName.Ic = 0;
            a.d.value = {
                after: ["options", "foreach"],
                init: function(b, c, d) {
                    if ("input" != b.tagName.toLowerCase() || "checkbox" != b.type && "radio" != b.type) {
                        var e = ["change"],
                            f = d.get("valueUpdate"),
                            g = !1,
                            k = null;
                        f && ("string" == typeof f && (f = [f]), a.a.ra(e, f), e = a.a.Tb(e));
                        var l = function() {
                            k = null;
                            g = !1;
                            var e = c(),
                                f = a.j.u(b);
                            a.h.Ea(e, d, "value", f)
                        };
                        !a.a.C || "input" != b.tagName.toLowerCase() || "text" != b.type ||
                            "off" == b.autocomplete || b.form && "off" == b.form.autocomplete || -1 != a.a.o(e, "propertychange") || (a.a.p(b, "propertychange", function() {
                                g = !0
                            }), a.a.p(b, "focus", function() {
                                g = !1
                            }), a.a.p(b, "blur", function() {
                                g && l()
                            }));
                        a.a.q(e, function(c) {
                            var d = l;
                            a.a.nd(c, "after") && (d = function() {
                                k = a.j.u(b);
                                a.a.setTimeout(l, 0)
                            }, c = c.substring(5));
                            a.a.p(b, c, d)
                        });
                        var m = function() {
                            var e = a.a.c(c()),
                                f = a.j.u(b);
                            if (null !== k && e === k) a.a.setTimeout(m, 0);
                            else if (e !== f)
                                if ("select" === a.a.A(b)) {
                                    var g = d.get("valueAllowUnset"),
                                        f = function() {
                                            a.j.ha(b,
                                                e, g)
                                        };
                                    f();
                                    g || e === a.j.u(b) ? a.a.setTimeout(f, 0) : a.l.w(a.a.Da, null, [b, "change"])
                                } else a.j.ha(b, e)
                        };
                        a.m(m, null, {
                            i: b
                        })
                    } else a.Ja(b, {
                        checkedValue: c
                    })
                },
                update: function() {}
            };
            a.h.ea.value = !0;
            a.d.visible = {
                update: function(b, c) {
                    var d = a.a.c(c()),
                        e = "none" != b.style.display;
                    d && !e ? b.style.display = "" : !d && e && (b.style.display = "none")
                }
            };
            (function(b) {
                a.d[b] = {
                    init: function(c, d, e, f, g) {
                        return a.d.event.init.call(this, c, function() {
                            var a = {};
                            a[b] = d();
                            return a
                        }, e, f, g)
                    }
                }
            })("click");
            a.O = function() {};
            a.O.prototype.renderTemplateSource =
                function() {
                    throw Error("Override renderTemplateSource");
                };
            a.O.prototype.createJavaScriptEvaluatorBlock = function() {
                throw Error("Override createJavaScriptEvaluatorBlock");
            };
            a.O.prototype.makeTemplateSource = function(b, c) {
                if ("string" == typeof b) {
                    c = c || u;
                    var d = c.getElementById(b);
                    if (!d) throw Error("Cannot find template with ID " + b);
                    return new a.v.n(d)
                }
                if (1 == b.nodeType || 8 == b.nodeType) return new a.v.qa(b);
                throw Error("Unknown template type: " + b);
            };
            a.O.prototype.renderTemplate = function(a, c, d, e) {
                a = this.makeTemplateSource(a,
                    e);
                return this.renderTemplateSource(a, c, d, e)
            };
            a.O.prototype.isTemplateRewritten = function(a, c) {
                return !1 === this.allowTemplateRewriting ? !0 : this.makeTemplateSource(a, c).data("isRewritten")
            };
            a.O.prototype.rewriteTemplate = function(a, c, d) {
                a = this.makeTemplateSource(a, d);
                c = c(a.text());
                a.text(c);
                a.data("isRewritten", !0)
            };
            a.b("templateEngine", a.O);
            a.Gb = function() {
                function b(b, c, d, k) {
                    b = a.h.yb(b);
                    for (var l = a.h.ta, m = 0; m < b.length; m++) {
                        var h = b[m].key;
                        if (l.hasOwnProperty(h)) {
                            var r = l[h];
                            if ("function" === typeof r) {
                                if (h =
                                    r(b[m].value)) throw Error(h);
                            } else if (!r) throw Error("This template engine does not support the '" + h + "' binding within its templates");
                        }
                    }
                    d = "ko.__tr_ambtns(function($context,$element){return(function(){return{ " + a.h.Ua(b, {
                        valueAccessors: !0
                    }) + " } })()},'" + d.toLowerCase() + "')";
                    return k.createJavaScriptEvaluatorBlock(d) + c
                }
                var c = /(<([a-z]+\d*)(?:\s+(?!data-bind\s*=\s*)[a-z0-9\-]+(?:=(?:\"[^\"]*\"|\'[^\']*\'|[^>]*))?)*\s+)data-bind\s*=\s*(["'])([\s\S]*?)\3/gi,
                    d = /\x3c!--\s*ko\b\s*([\s\S]*?)\s*--\x3e/g;
                return {
                    Oc: function(b,
                        c, d) {
                        c.isTemplateRewritten(b, d) || c.rewriteTemplate(b, function(b) {
                            return a.Gb.dd(b, c)
                        }, d)
                    },
                    dd: function(a, f) {
                        return a.replace(c, function(a, c, d, e, h) {
                            return b(h, c, d, f)
                        }).replace(d, function(a, c) {
                            return b(c, "\x3c!-- ko --\x3e", "#comment", f)
                        })
                    },
                    Ec: function(b, c) {
                        return a.M.wb(function(d, k) {
                            var l = d.nextSibling;
                            l && l.nodeName.toLowerCase() === c && a.Ja(l, b, k)
                        })
                    }
                }
            }();
            a.b("__tr_ambtns", a.Gb.Ec);
            (function() {
                a.v = {};
                a.v.n = function(b) {
                    if (this.n = b) {
                        var c = a.a.A(b);
                        this.ab = "script" === c ? 1 : "textarea" === c ? 2 : "template" == c &&
                            b.content && 11 === b.content.nodeType ? 3 : 4
                    }
                };
                a.v.n.prototype.text = function() {
                    var b = 1 === this.ab ? "text" : 2 === this.ab ? "value" : "innerHTML";
                    if (0 == arguments.length) return this.n[b];
                    var c = arguments[0];
                    "innerHTML" === b ? a.a.Cb(this.n, c) : this.n[b] = c
                };
                var b = a.a.e.I() + "_";
                a.v.n.prototype.data = function(c) {
                    if (1 === arguments.length) return a.a.e.get(this.n, b + c);
                    a.a.e.set(this.n, b + c, arguments[1])
                };
                var c = a.a.e.I();
                a.v.n.prototype.nodes = function() {
                    var b = this.n;
                    if (0 == arguments.length) return (a.a.e.get(b, c) || {}).jb || (3 === this.ab ?
                        b.content : 4 === this.ab ? b : n);
                    a.a.e.set(b, c, {
                        jb: arguments[0]
                    })
                };
                a.v.qa = function(a) {
                    this.n = a
                };
                a.v.qa.prototype = new a.v.n;
                a.v.qa.prototype.text = function() {
                    if (0 == arguments.length) {
                        var b = a.a.e.get(this.n, c) || {};
                        b.Hb === n && b.jb && (b.Hb = b.jb.innerHTML);
                        return b.Hb
                    }
                    a.a.e.set(this.n, c, {
                        Hb: arguments[0]
                    })
                };
                a.b("templateSources", a.v);
                a.b("templateSources.domElement", a.v.n);
                a.b("templateSources.anonymousTemplate", a.v.qa)
            })();
            (function() {
                function b(b, c, d) {
                    var e;
                    for (c = a.f.nextSibling(c); b && (e = b) !== c;) b = a.f.nextSibling(e),
                        d(e, b)
                }

                function c(c, d) {
                    if (c.length) {
                        var e = c[0],
                            f = c[c.length - 1],
                            g = e.parentNode,
                            k = a.Q.instance,
                            n = k.preprocessNode;
                        if (n) {
                            b(e, f, function(a, b) {
                                var c = a.previousSibling,
                                    d = n.call(k, a);
                                d && (a === e && (e = d[0] || b), a === f && (f = d[d.length - 1] || c))
                            });
                            c.length = 0;
                            if (!e) return;
                            e === f ? c.push(e) : (c.push(e, f), a.a.za(c, g))
                        }
                        b(e, f, function(b) {
                            1 !== b.nodeType && 8 !== b.nodeType || a.Rb(d, b)
                        });
                        b(e, f, function(b) {
                            1 !== b.nodeType && 8 !== b.nodeType || a.M.yc(b, [d])
                        });
                        a.a.za(c, g)
                    }
                }

                function d(a) {
                    return a.nodeType ? a : 0 < a.length ? a[0] : null
                }

                function e(b,
                    e, f, k, q) {
                    q = q || {};
                    var p = (b && d(b) || f || {}).ownerDocument,
                        n = q.templateEngine || g;
                    a.Gb.Oc(f, n, p);
                    f = n.renderTemplate(f, k, q, p);
                    if ("number" != typeof f.length || 0 < f.length && "number" != typeof f[0].nodeType) throw Error("Template engine must return an array of DOM nodes");
                    p = !1;
                    switch (e) {
                        case "replaceChildren":
                            a.f.da(b, f);
                            p = !0;
                            break;
                        case "replaceNode":
                            a.a.qc(b, f);
                            p = !0;
                            break;
                        case "ignoreTargetNode":
                            break;
                        default:
                            throw Error("Unknown renderMode: " + e);
                    }
                    p && (c(f, k), q.afterRender && a.l.w(q.afterRender, null, [f, k.$data]));
                    return f
                }

                function f(b, c, d) {
                    return a.H(b) ? b() : "function" === typeof b ? b(c, d) : b
                }
                var g;
                a.Db = function(b) {
                    if (b != n && !(b instanceof a.O)) throw Error("templateEngine must inherit from ko.templateEngine");
                    g = b
                };
                a.Ab = function(b, c, h, k, q) {
                    h = h || {};
                    if ((h.templateEngine || g) == n) throw Error("Set a template engine before calling renderTemplate");
                    q = q || "replaceChildren";
                    if (k) {
                        var p = d(k);
                        return a.B(function() {
                            var g = c && c instanceof a.U ? c : new a.U(a.a.c(c)),
                                n = f(b, g.$data, g),
                                g = e(k, q, n, g, h);
                            "replaceNode" == q && (k = g, p = d(k))
                        }, null, {
                            wa: function() {
                                return !p || !a.a.nb(p)
                            },
                            i: p && "replaceNode" == q ? p.parentNode : p
                        })
                    }
                    return a.M.wb(function(d) {
                        a.Ab(b, c, h, d, "replaceNode")
                    })
                };
                a.kd = function(b, d, g, k, q) {
                    function p(a, b) {
                        c(b, s);
                        g.afterRender && g.afterRender(b, a);
                        s = null
                    }

                    function u(a, c) {
                        s = q.createChildContext(a, g.as, function(a) {
                            a.$index = c
                        });
                        var d = f(b, a, s);
                        return e(null, "ignoreTargetNode", d, s, g)
                    }
                    var s;
                    return a.B(function() {
                        var b = a.a.c(d) || [];
                        "undefined" == typeof b.length && (b = [b]);
                        b = a.a.Ka(b, function(b) {
                            return g.includeDestroyed || b === n || null === b || !a.a.c(b._destroy)
                        });
                        a.l.w(a.a.Bb, null, [k, b, u, g, p])
                    }, null, {
                        i: k
                    })
                };
                var k = a.a.e.I();
                a.d.template = {
                    init: function(b, c) {
                        var d = a.a.c(c());
                        if ("string" == typeof d || d.name) a.f.xa(b);
                        else {
                            if ("nodes" in d) {
                                if (d = d.nodes || [], a.H(d)) throw Error('The "nodes" option must be a plain, non-observable array.');
                            } else d = a.f.childNodes(b);
                            d = a.a.jc(d);
                            (new a.v.qa(b)).nodes(d)
                        }
                        return {
                            controlsDescendantBindings: !0
                        }
                    },
                    update: function(b, c, d, e, f) {
                        var g = c(),
                            s;
                        c = a.a.c(g);
                        d = !0;
                        e = null;
                        "string" == typeof c ? c = {} : (g = c.name, "if" in c && (d = a.a.c(c["if"])), d && "ifnot" in
                            c && (d = !a.a.c(c.ifnot)), s = a.a.c(c.data));
                        "foreach" in c ? e = a.kd(g || b, d && c.foreach || [], c, b, f) : d ? (f = "data" in c ? f.createChildContext(s, c.as) : f, e = a.Ab(g || b, f, c, b)) : a.f.xa(b);
                        f = e;
                        (s = a.a.e.get(b, k)) && "function" == typeof s.k && s.k();
                        a.a.e.set(b, k, f && f.ba() ? f : n)
                    }
                };
                a.h.ta.template = function(b) {
                    b = a.h.yb(b);
                    return 1 == b.length && b[0].unknown || a.h.ad(b, "name") ? null : "This template engine does not support anonymous templates nested within its templates"
                };
                a.f.Z.template = !0
            })();
            a.b("setTemplateEngine", a.Db);
            a.b("renderTemplate",
                a.Ab);
            a.a.dc = function(a, c, d) {
                if (a.length && c.length) {
                    var e, f, g, k, l;
                    for (e = f = 0;
                        (!d || e < d) && (k = a[f]); ++f) {
                        for (g = 0; l = c[g]; ++g)
                            if (k.value === l.value) {
                                k.moved = l.index;
                                l.moved = k.index;
                                c.splice(g, 1);
                                e = g = 0;
                                break
                            } e += g
                    }
                }
            };
            a.a.ib = function() {
                function b(b, d, e, f, g) {
                    var k = Math.min,
                        l = Math.max,
                        m = [],
                        h, n = b.length,
                        q, p = d.length,
                        s = p - n || 1,
                        u = n + p + 1,
                        t, v, x;
                    for (h = 0; h <= n; h++)
                        for (v = t, m.push(t = []), x = k(p, h + s), q = l(0, h - 1); q <= x; q++) t[q] = q ? h ? b[h - 1] === d[q - 1] ? v[q - 1] : k(v[q] || u, t[q - 1] || u) + 1 : q + 1 : h + 1;
                    k = [];
                    l = [];
                    s = [];
                    h = n;
                    for (q = p; h || q;) p = m[h][q] -
                        1, q && p === m[h][q - 1] ? l.push(k[k.length] = {
                            status: e,
                            value: d[--q],
                            index: q
                        }) : h && p === m[h - 1][q] ? s.push(k[k.length] = {
                            status: f,
                            value: b[--h],
                            index: h
                        }) : (--q, --h, g.sparse || k.push({
                            status: "retained",
                            value: d[q]
                        }));
                    a.a.dc(s, l, !g.dontLimitMoves && 10 * n);
                    return k.reverse()
                }
                return function(a, d, e) {
                    e = "boolean" === typeof e ? {
                        dontLimitMoves: e
                    } : e || {};
                    a = a || [];
                    d = d || [];
                    return a.length < d.length ? b(a, d, "added", "deleted", e) : b(d, a, "deleted", "added", e)
                }
            }();
            a.b("utils.compareArrays", a.a.ib);
            (function() {
                function b(b, c, d, k, l) {
                    var m = [],
                        h = a.B(function() {
                            var h = c(d, l, a.a.za(m, b)) || [];
                            0 < m.length && (a.a.qc(m, h), k && a.l.w(k, null, [d, h, l]));
                            m.length = 0;
                            a.a.ra(m, h)
                        }, null, {
                            i: b,
                            wa: function() {
                                return !a.a.Qb(m)
                            }
                        });
                    return {
                        ca: m,
                        B: h.ba() ? h : n
                    }
                }
                var c = a.a.e.I(),
                    d = a.a.e.I();
                a.a.Bb = function(e, f, g, k, l) {
                    function m(b, c) {
                        w = q[c];
                        v !== c && (D[b] = w);
                        w.qb(v++);
                        a.a.za(w.ca, e);
                        u.push(w);
                        z.push(w)
                    }

                    function h(b, c) {
                        if (b)
                            for (var d = 0, e = c.length; d < e; d++) c[d] && a.a.q(c[d].ca, function(a) {
                                b(a, d, c[d].ja)
                            })
                    }
                    f = f || [];
                    k = k || {};
                    var r = a.a.e.get(e, c) === n,
                        q = a.a.e.get(e, c) || [],
                        p = a.a.fb(q,
                            function(a) {
                                return a.ja
                            }),
                        s = a.a.ib(p, f, k.dontLimitMoves),
                        u = [],
                        t = 0,
                        v = 0,
                        x = [],
                        z = [];
                    f = [];
                    for (var D = [], p = [], w, C = 0, B, E; B = s[C]; C++) switch (E = B.moved, B.status) {
                        case "deleted":
                            E === n && (w = q[t], w.B && (w.B.k(), w.B = n), a.a.za(w.ca, e).length && (k.beforeRemove && (u.push(w), z.push(w), w.ja === d ? w = null : f[C] = w), w && x.push.apply(x, w.ca)));
                            t++;
                            break;
                        case "retained":
                            m(C, t++);
                            break;
                        case "added":
                            E !== n ? m(C, E) : (w = {
                                ja: B.value,
                                qb: a.N(v++)
                            }, u.push(w), z.push(w), r || (p[C] = w))
                    }
                    a.a.e.set(e, c, u);
                    h(k.beforeMove, D);
                    a.a.q(x, k.beforeRemove ? a.$ :
                        a.removeNode);
                    for (var C = 0, r = a.f.firstChild(e), F; w = z[C]; C++) {
                        w.ca || a.a.extend(w, b(e, g, w.ja, l, w.qb));
                        for (t = 0; s = w.ca[t]; r = s.nextSibling, F = s, t++) s !== r && a.f.gc(e, s, F);
                        !w.Wc && l && (l(w.ja, w.ca, w.qb), w.Wc = !0)
                    }
                    h(k.beforeRemove, f);
                    for (C = 0; C < f.length; ++C) f[C] && (f[C].ja = d);
                    h(k.afterMove, D);
                    h(k.afterAdd, p)
                }
            })();
            a.b("utils.setDomNodeChildrenFromArrayMapping", a.a.Bb);
            a.W = function() {
                this.allowTemplateRewriting = !1
            };
            a.W.prototype = new a.O;
            a.W.prototype.renderTemplateSource = function(b, c, d, e) {
                if (c = (9 > a.a.C ? 0 : b.nodes) ?
                    b.nodes() : null) return a.a.V(c.cloneNode(!0).childNodes);
                b = b.text();
                return a.a.ma(b, e)
            };
            a.W.sb = new a.W;
            a.Db(a.W.sb);
            a.b("nativeTemplateEngine", a.W);
            (function() {
                a.vb = function() {
                    var a = this.$c = function() {
                        if (!v || !v.tmpl) return 0;
                        try {
                            if (0 <= v.tmpl.tag.tmpl.open.toString().indexOf("__")) return 2
                        } catch (a) {}
                        return 1
                    }();
                    this.renderTemplateSource = function(b, e, f, g) {
                        g = g || u;
                        f = f || {};
                        if (2 > a) throw Error("Your version of jQuery.tmpl is too old. Please upgrade to jQuery.tmpl 1.0.0pre or later.");
                        var k = b.data("precompiled");
                        k || (k = b.text() || "", k = v.template(null, "{{ko_with $item.koBindingContext}}" + k + "{{/ko_with}}"), b.data("precompiled", k));
                        b = [e.$data];
                        e = v.extend({
                            koBindingContext: e
                        }, f.templateOptions);
                        e = v.tmpl(k, b, e);
                        e.appendTo(g.createElement("div"));
                        v.fragments = {};
                        return e
                    };
                    this.createJavaScriptEvaluatorBlock = function(a) {
                        return "{{ko_code ((function() { return " + a + " })()) }}"
                    };
                    this.addTemplate = function(a, b) {
                        u.write("<script type='text/html' id='" + a + "'>" + b + "\x3c/script>")
                    };
                    0 < a && (v.tmpl.tag.ko_code = {
                            open: "__.push($1 || '');"
                        },
                        v.tmpl.tag.ko_with = {
                            open: "with($1) {",
                            close: "} "
                        })
                };
                a.vb.prototype = new a.O;
                var b = new a.vb;
                0 < b.$c && a.Db(b);
                a.b("jqueryTmplTemplateEngine", a.vb)
            })()
        })
    })();
})();
/// Knockout Mapping plugin v2.4.1
/// (c) 2013 Steven Sanderson, Roy Jacobs - http://knockoutjs.com/
/// License: MIT (http://www.opensource.org/licenses/mit-license.php)
(function(e) {
    "function" === typeof require && "object" === typeof exports && "object" === typeof module ? e(require("knockout"), exports) : "function" === typeof define && define.amd ? define(["knockout", "exports"], e) : e(ko, ko.mapping = {})
})(function(e, f) {
    function y(b, c) {
        var a, d;
        for (d in c)
            if (c.hasOwnProperty(d) && c[d])
                if (a = f.getType(b[d]), d && b[d] && "array" !== a && "string" !== a) y(b[d], c[d]);
                else if ("array" === f.getType(b[d]) && "array" === f.getType(c[d])) {
            a = b;
            for (var e = d, l = b[d], n = c[d], t = {}, g = l.length - 1; 0 <= g; --g) t[l[g]] = l[g];
            for (g =
                n.length - 1; 0 <= g; --g) t[n[g]] = n[g];
            l = [];
            n = void 0;
            for (n in t) l.push(t[n]);
            a[e] = l
        } else b[d] = c[d]
    }

    function E(b, c) {
        var a = {};
        y(a, b);
        y(a, c);
        return a
    }

    function z(b, c) {
        for (var a = E({}, b), e = L.length - 1; 0 <= e; e--) {
            var f = L[e];
            a[f] && (a[""] instanceof Object || (a[""] = {}), a[""][f] = a[f], delete a[f])
        }
        c && (a.ignore = h(c.ignore, a.ignore), a.include = h(c.include, a.include), a.copy = h(c.copy, a.copy), a.observe = h(c.observe, a.observe));
        a.ignore = h(a.ignore, j.ignore);
        a.include = h(a.include, j.include);
        a.copy = h(a.copy, j.copy);
        a.observe = h(a.observe,
            j.observe);
        a.mappedProperties = a.mappedProperties || {};
        a.copiedProperties = a.copiedProperties || {};
        return a
    }

    function h(b, c) {
        "array" !== f.getType(b) && (b = "undefined" === f.getType(b) ? [] : [b]);
        "array" !== f.getType(c) && (c = "undefined" === f.getType(c) ? [] : [c]);
        return e.utils.arrayGetDistinctValues(b.concat(c))
    }

    function F(b, c, a, d, k, l, n) {
        var t = "array" === f.getType(e.utils.unwrapObservable(c));
        l = l || "";
        if (f.isMapped(b)) {
            var g = e.utils.unwrapObservable(b)[p];
            a = E(g, a)
        }
        var j = n || k,
            h = function() {
                return a[d] && a[d].create instanceof
                Function
            },
            x = function(b) {
                var f = G,
                    g = e.dependentObservable;
                e.dependentObservable = function(a, b, c) {
                    c = c || {};
                    a && "object" == typeof a && (c = a);
                    var d = c.deferEvaluation,
                        M = !1;
                    c.deferEvaluation = !0;
                    a = new H(a, b, c);
                    if (!d) {
                        var g = a,
                            d = e.dependentObservable;
                        e.dependentObservable = H;
                        a = e.isWriteableObservable(g);
                        e.dependentObservable = d;
                        d = H({
                            read: function() {
                                M || (e.utils.arrayRemoveItem(f, g), M = !0);
                                return g.apply(g, arguments)
                            },
                            write: a && function(a) {
                                return g(a)
                            },
                            deferEvaluation: !0
                        });
                        d.__DO = g;
                        a = d;
                        f.push(a)
                    }
                    return a
                };
                e.dependentObservable.fn =
                    H.fn;
                e.computed = e.dependentObservable;
                b = e.utils.unwrapObservable(k) instanceof Array ? a[d].create({
                    data: b || c,
                    parent: j,
                    skip: N
                }) : a[d].create({
                    data: b || c,
                    parent: j
                });
                e.dependentObservable = g;
                e.computed = e.dependentObservable;
                return b
            },
            u = function() {
                return a[d] && a[d].update instanceof Function
            },
            v = function(b, f) {
                var g = {
                    data: f || c,
                    parent: j,
                    target: e.utils.unwrapObservable(b)
                };
                e.isWriteableObservable(b) && (g.observable = b);
                return a[d].update(g)
            };
        if (n = I.get(c)) return n;
        d = d || "";
        if (t) {
            var t = [],
                s = !1,
                m = function(a) {
                    return a
                };
            a[d] && a[d].key && (m = a[d].key, s = !0);
            e.isObservable(b) || (b = e.observableArray([]), b.mappedRemove = function(a) {
                var c = "function" == typeof a ? a : function(b) {
                    return b === m(a)
                };
                return b.remove(function(a) {
                    return c(m(a))
                })
            }, b.mappedRemoveAll = function(a) {
                var c = C(a, m);
                return b.remove(function(a) {
                    return -1 != e.utils.arrayIndexOf(c, m(a))
                })
            }, b.mappedDestroy = function(a) {
                var c = "function" == typeof a ? a : function(b) {
                    return b === m(a)
                };
                return b.destroy(function(a) {
                    return c(m(a))
                })
            }, b.mappedDestroyAll = function(a) {
                var c = C(a, m);
                return b.destroy(function(a) {
                    return -1 !=
                        e.utils.arrayIndexOf(c, m(a))
                })
            }, b.mappedIndexOf = function(a) {
                var c = C(b(), m);
                a = m(a);
                return e.utils.arrayIndexOf(c, a)
            }, b.mappedGet = function(a) {
                return b()[b.mappedIndexOf(a)]
            }, b.mappedCreate = function(a) {
                if (-1 !== b.mappedIndexOf(a)) throw Error("There already is an object with the key that you specified.");
                var c = h() ? x(a) : a;
                u() && (a = v(c, a), e.isWriteableObservable(c) ? c(a) : c = a);
                b.push(c);
                return c
            });
            n = C(e.utils.unwrapObservable(b), m).sort();
            g = C(c, m);
            s && g.sort();
            s = e.utils.compareArrays(n, g);
            n = {};
            var J, A = e.utils.unwrapObservable(c),
                y = {},
                z = !0,
                g = 0;
            for (J = A.length; g < J; g++) {
                var r = m(A[g]);
                if (void 0 === r || r instanceof Object) {
                    z = !1;
                    break
                }
                y[r] = A[g]
            }
            var A = [],
                B = 0,
                g = 0;
            for (J = s.length; g < J; g++) {
                var r = s[g],
                    q, w = l + "[" + g + "]";
                switch (r.status) {
                    case "added":
                        var D = z ? y[r.value] : K(e.utils.unwrapObservable(c), r.value, m);
                        q = F(void 0, D, a, d, b, w, k);
                        h() || (q = e.utils.unwrapObservable(q));
                        w = O(e.utils.unwrapObservable(c), D, n);
                        q === N ? B++ : A[w - B] = q;
                        n[w] = !0;
                        break;
                    case "retained":
                        D = z ? y[r.value] : K(e.utils.unwrapObservable(c), r.value, m);
                        q = K(b, r.value, m);
                        F(q, D, a, d, b, w,
                            k);
                        w = O(e.utils.unwrapObservable(c), D, n);
                        A[w] = q;
                        n[w] = !0;
                        break;
                    case "deleted":
                        q = K(b, r.value, m)
                }
                t.push({
                    event: r.status,
                    item: q
                })
            }
            b(A);
            a[d] && a[d].arrayChanged && e.utils.arrayForEach(t, function(b) {
                a[d].arrayChanged(b.event, b.item)
            })
        } else if (P(c)) {
            b = e.utils.unwrapObservable(b);
            if (!b) {
                if (h()) return s = x(), u() && (s = v(s)), s;
                if (u()) return v(s);
                b = {}
            }
            u() && (b = v(b));
            I.save(c, b);
            if (u()) return b;
            Q(c, function(d) {
                var f = l.length ? l + "." + d : d;
                if (-1 == e.utils.arrayIndexOf(a.ignore, f))
                    if (-1 != e.utils.arrayIndexOf(a.copy, f)) b[d] =
                        c[d];
                    else if ("object" != typeof c[d] && "array" != typeof c[d] && 0 < a.observe.length && -1 == e.utils.arrayIndexOf(a.observe, f)) b[d] = c[d], a.copiedProperties[f] = !0;
                else {
                    var g = I.get(c[d]),
                        k = F(b[d], c[d], a, d, b, f, b),
                        g = g || k;
                    if (0 < a.observe.length && -1 == e.utils.arrayIndexOf(a.observe, f)) b[d] = g(), a.copiedProperties[f] = !0;
                    else {
                        if (e.isWriteableObservable(b[d])) {
                            if (g = e.utils.unwrapObservable(g), b[d]() !== g) b[d](g)
                        } else g = void 0 === b[d] ? g : e.utils.unwrapObservable(g), b[d] = g;
                        a.mappedProperties[f] = !0
                    }
                }
            })
        } else switch (f.getType(c)) {
            case "function":
                u() ?
                    e.isWriteableObservable(c) ? (c(v(c)), b = c) : b = v(c) : b = c;
                break;
            default:
                if (e.isWriteableObservable(b)) return q = u() ? v(b) : e.utils.unwrapObservable(c), b(q), q;
                h() || u();
                b = h() ? x() : e.observable(e.utils.unwrapObservable(c));
                u() && b(v(b))
        }
        return b
    }

    function O(b, c, a) {
        for (var d = 0, e = b.length; d < e; d++)
            if (!0 !== a[d] && b[d] === c) return d;
        return null
    }

    function R(b, c) {
        var a;
        c && (a = c(b));
        "undefined" === f.getType(a) && (a = b);
        return e.utils.unwrapObservable(a)
    }

    function K(b, c, a) {
        b = e.utils.unwrapObservable(b);
        for (var d = 0, f = b.length; d <
            f; d++) {
            var l = b[d];
            if (R(l, a) === c) return l
        }
        throw Error("When calling ko.update*, the key '" + c + "' was not found!");
    }

    function C(b, c) {
        return e.utils.arrayMap(e.utils.unwrapObservable(b), function(a) {
            return c ? R(a, c) : a
        })
    }

    function Q(b, c) {
        if ("array" === f.getType(b))
            for (var a = 0; a < b.length; a++) c(a);
        else
            for (a in b) c(a)
    }

    function P(b) {
        var c = f.getType(b);
        return ("object" === c || "array" === c) && null !== b
    }

    function T() {
        var b = [],
            c = [];
        this.save = function(a, d) {
            var f = e.utils.arrayIndexOf(b, a);
            0 <= f ? c[f] = d : (b.push(a), c.push(d))
        };
        this.get = function(a) {
            a = e.utils.arrayIndexOf(b, a);
            return 0 <= a ? c[a] : void 0
        }
    }

    function S() {
        var b = {},
            c = function(a) {
                var c;
                try {
                    c = a
                } catch (e) {
                    c = "$$$"
                }
                a = b[c];
                void 0 === a && (a = new T, b[c] = a);
                return a
            };
        this.save = function(a, b) {
            c(a).save(a, b)
        };
        this.get = function(a) {
            return c(a).get(a)
        }
    }
    var p = "__ko_mapping__",
        H = e.dependentObservable,
        B = 0,
        G, I, L = ["create", "update", "key", "arrayChanged"],
        N = {},
        x = {
            include: ["_destroy"],
            ignore: [],
            copy: [],
            observe: []
        },
        j = x;
    f.isMapped = function(b) {
        return (b = e.utils.unwrapObservable(b)) && b[p]
    };
    f.fromJS =
        function(b) {
            if (0 == arguments.length) throw Error("When calling ko.fromJS, pass the object you want to convert.");
            try {
                B++ || (G = [], I = new S);
                var c, a;
                2 == arguments.length && (arguments[1][p] ? a = arguments[1] : c = arguments[1]);
                3 == arguments.length && (c = arguments[1], a = arguments[2]);
                a && (c = E(c, a[p]));
                c = z(c);
                var d = F(a, b, c);
                a && (d = a);
                if (!--B)
                    for (; G.length;) {
                        var e = G.pop();
                        e && (e(), e.__DO.throttleEvaluation = e.throttleEvaluation)
                    }
                d[p] = E(d[p], c);
                return d
            } catch (f) {
                throw B = 0, f;
            }
        };
    f.fromJSON = function(b) {
        var c = e.utils.parseJson(b);
        arguments[0] = c;
        return f.fromJS.apply(this, arguments)
    };
    f.updateFromJS = function() {
        throw Error("ko.mapping.updateFromJS, use ko.mapping.fromJS instead. Please note that the order of parameters is different!");
    };
    f.updateFromJSON = function() {
        throw Error("ko.mapping.updateFromJSON, use ko.mapping.fromJSON instead. Please note that the order of parameters is different!");
    };
    f.toJS = function(b, c) {
        j || f.resetDefaultOptions();
        if (0 == arguments.length) throw Error("When calling ko.mapping.toJS, pass the object you want to convert.");
        if ("array" !== f.getType(j.ignore)) throw Error("ko.mapping.defaultOptions().ignore should be an array.");
        if ("array" !== f.getType(j.include)) throw Error("ko.mapping.defaultOptions().include should be an array.");
        if ("array" !== f.getType(j.copy)) throw Error("ko.mapping.defaultOptions().copy should be an array.");
        c = z(c, b[p]);
        return f.visitModel(b, function(a) {
            return e.utils.unwrapObservable(a)
        }, c)
    };
    f.toJSON = function(b, c) {
        var a = f.toJS(b, c);
        return e.utils.stringifyJson(a)
    };
    f.defaultOptions = function() {
        if (0 < arguments.length) j =
            arguments[0];
        else return j
    };
    f.resetDefaultOptions = function() {
        j = {
            include: x.include.slice(0),
            ignore: x.ignore.slice(0),
            copy: x.copy.slice(0)
        }
    };
    f.getType = function(b) {
        if (b && "object" === typeof b) {
            if (b.constructor === Date) return "date";
            if (b.constructor === Array) return "array"
        }
        return typeof b
    };
    f.visitModel = function(b, c, a) {
        a = a || {};
        a.visitedObjects = a.visitedObjects || new S;
        var d, k = e.utils.unwrapObservable(b);
        if (P(k)) a = z(a, k[p]), c(b, a.parentName), d = "array" === f.getType(k) ? [] : {};
        else return c(b, a.parentName);
        a.visitedObjects.save(b,
            d);
        var l = a.parentName;
        Q(k, function(b) {
            if (!(a.ignore && -1 != e.utils.arrayIndexOf(a.ignore, b))) {
                var j = k[b],
                    g = a,
                    h = l || "";
                "array" === f.getType(k) ? l && (h += "[" + b + "]") : (l && (h += "."), h += b);
                g.parentName = h;
                if (!(-1 === e.utils.arrayIndexOf(a.copy, b) && -1 === e.utils.arrayIndexOf(a.include, b) && k[p] && k[p].mappedProperties && !k[p].mappedProperties[b] && k[p].copiedProperties && !k[p].copiedProperties[b] && "array" !== f.getType(k))) switch (f.getType(e.utils.unwrapObservable(j))) {
                    case "object":
                    case "array":
                    case "undefined":
                        g = a.visitedObjects.get(j);
                        d[b] = "undefined" !== f.getType(g) ? g : f.visitModel(j, c, a);
                        break;
                    default:
                        d[b] = c(j, a.parentName)
                }
            }
        });
        return d
    }
});
