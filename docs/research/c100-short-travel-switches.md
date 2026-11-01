# Low-travel switches for the Keychron C100 8K

Research date: 3 September 2026

## Bottom line

The C100 uses the **normal-profile Cherry-MX-family mechanical interface**. “Standard hot-swappable” does not mean universal: a replacement must match the mechanical contact system, PCB/socket footprint, plate geometry and keycap clearance.

The stock Keychron Apex Red is a conventional linear switch with approximately **2.0 mm pretravel and 4.0 mm total travel**. For a materially shorter stroke and a lower key height, the best-supported C100 candidate is the **Tecsee Pudding Medium/Middle**. Buy samples before ordering 100 because the C100’s transparent ASA keycaps may collide with its shortened housing.

## What “standard” means on this board

- An ordinary metal-contact mechanical switch—not optical or magnetic/HE/TMR.
- The normal MX contact-pin and center-post arrangement used by MX hot-swap sockets.
- A normal MX plate-mount housing and normal-profile PCB-to-plate spacing.
- Either three-pin or five-pin construction: five-pin merely adds two plastic positioning legs.
- An MX-cross keycap stem.
- An opening/light path suitable for the C100’s north-facing SMD RGB if lighting matters.

[Keychron’s C100 page](https://www.keychron.com/products/keychron-c100-8k-giant-custom-macro-pad) confirms Apex Red, hot swapping, transparent ASA caps and north-facing RGB, but it does not explicitly print “3-pin/5-pin” or provide a plate drawing. The conclusion that this is ordinary MX 3-/5-pin is therefore an inference from the Apex platform and Keychron’s other Apex-equipped C-series boards; the [C2 Pro 8K specification](https://www.keychron.com/collections/rgb-backlit-keyboard/products/keychron-c2-pro-8k-qmk-via-wired-mechanical-keyboard?page=2), for example, explicitly says 3-/5-pin MX.

An “MX-style stem” only tells you that an MX keycap may attach. It says nothing about the electrical pins or plate fit.

## Best candidates

| Switch | Physical profile | Feel | Pretravel | Total travel | C100 assessment |
|---|---|---|---:|---:|---|
| Tecsee Pudding Medium/Middle | Shortened MX / medium | Linear or tactile | 1.3 mm | 2.2–2.5 mm | Best height-and-travel candidate; sample-test ASA cap clearance |
| Outemu/Gazzew GTMX | Shortened MX / medium | Linear, tactile or clicky | 1.0–1.2 mm | 3.0 mm | MX compatible, but scarce and potentially very tight in plates |
| TTC Mini Purple | Low profile | Linear | 1.3 mm | 3.2–3.5 mm | Same-socket claim, but C100 plate stack unverified; little travel benefit |
| Akko V3 Creamy Purple Pro | Normal height | Tactile | 2.0 mm | 3.0 mm | Low-risk normal-MX choice; stocked in EU |
| PUNK WORKSHOP PWS V2 | Normal/arcade-oriented | Linear | 1.0 mm | 2.0 mm | Ultrashort experiment; not C100/RGB/keycap validated |
| PUNK WORKSHOP PWS V1 | Normal/arcade-oriented | Linear | 0.7 mm | 1.5 mm | Shortest found, but manufacturer warns of possible false inputs |

### Tecsee Pudding Medium/Middle

Tecsee publishes a five-pin MX-compatible bottom, SMD/through-hole lighting support, 50 gf actuation, 1.3 mm pretravel and 2.5 mm total travel. Cherry Xtrfy’s listing says 2.2 mm, so the defensible figure is **2.2–2.5 mm depending on listing or revision**. See [Tecsee’s current product page](https://tecseekeys.store/products/tecsee-middle-switch) and [Cherry Xtrfy’s specification](https://cherryxtrfy.com/keyboard-switches/tecsee-medium-linear-switches/).

It is the clearest candidate whose lower housing retains the normal MX footprint and plate interface while shortening the upper housing. The C100’s all-1u layout avoids stabilizer problems.

The risk is keycap collision. Shortening the housing lets the keycap skirt approach the plate, and the stock C100 cap is ASA. Published compatibility observations specifically flag some ASA molds. Profile names alone are not enough because skirt thickness, ribs and stem depth vary. Test a few switches with the actual stock caps before committing to 100.

### Outemu/Gazzew GTMX

The [SplitKB listing](https://splitkb.com/products/gazzew-low-profile-switches) describes a normal-MX lower half, 2 mm height reduction, 1 mm actuation and 3 mm total travel. [Adafruit’s heavy-linear version](https://www.adafruit.com/product/6022) is 3 mm lower above the board, 1.2/3.0 mm travel and 70±10 gf.

This is a real MX-footprint option, but the European prototype stock is sold out and other stock is fragmented. Keycap clearance and difficult removal from tight plates are additional reasons not to make it the first choice.

### TTC Mini

TTC says the Mini uses the “same keyswitch socket” and same MX keycaps, and reduces exposed installation height from 15.35 to 9.0 mm. The [TTC Mini Purple specification](https://en.ttc9.com/productdetails/7.html) gives 1.3 mm pretravel and up to 3.5 mm total travel.

That does not establish compatibility with the C100’s fixed plate height, retention points or ASA cap. It is worth a one-switch experiment if lowering the key surface matters, but its total travel is longer than Tecsee, GTMX or several normal-height short-travel switches.

### PUNK WORKSHOP PWS V1/V2

PUNK WORKSHOP’s [Japanese compatibility notice](https://punkworkshop.jp/blogs/updates/pws-switch-release) calls V1/V2 Cherry-MX compatible. Its [global specification](https://pws.top/products/pws-switch) gives V1 0.7/1.5 mm and V2 1.0/2.0 mm pretravel/total travel.

These are designed for arcade buttons rather than ordinary keyboard use. Pin count, C100 RGB performance and stock-keycap clearance are not sufficiently documented, so they are experiments—not safe 100-switch recommendations. V2 is the more sensible test because the manufacturer itself recommends V2 when V1 produces unintended inputs.

## Low-profile switches that should not be installed

- Keychron/Gateron KS-27 and KS-33 2.0 switches
- Most Gateron KS-33 3.0 switches
- Kailh Choc V1 or Choc V2/PG1353
- Cherry MX Low Profile
- Any optical, Hall-effect or TMR switch

These use different footprints, plate stacks or sensing systems. Keychron’s own [low-profile compatibility guidance](https://www.keychron.com/collections/low-profile-switches) says its LP switches have a different pin layout and height and only work in specifically designed LP keyboards. Kailh’s [Choc socket documentation](https://www.kailhswitch.com/mechanical-keyboard-switches/box-switches/choc-type-hot-swap-socket.html) likewise identifies a dedicated Choc footprint.

Gateron Grey Heron LP 3.0 is an especially confusing near-miss. Gateron says its contact-pin structure matches high-profile hot-swap terminals, but its own [product page](https://www.gateron.com/products/gateron-low-profile-grey-heron-switch) limits it to specific NuPhy keyboards. Matching electrical contacts does not solve the center-post, plate-height and retention geometry, and its 3.5 mm travel is not particularly short.

## Practical recommendation

1. Order several **Tecsee Pudding Medium Linear and Tactile** samples.
2. Test plate seating, electrical registration, full return, full bottom-out with the stock transparent ASA cap, and RGB visibility.
3. If the cap collides, try a verified compatible keycap mold or choose a normal-height short-travel switch.
4. If height does not matter, the **Akko V3 Creamy Purple Pro** is a straightforward 3.0 mm tactile option currently listed by [Akko Germany](https://akkogear.de/en/products/akko-v3-creamy-purple-pro-switch-45pcs). For a linear feel, Akko Silver-family switches around 3.0–3.3 mm are the conventional alternative.

No other currently obtainable, well-documented, modification-free physically lower MX-footprint family was found beyond Tecsee Medium and the scarce GTMX family.
