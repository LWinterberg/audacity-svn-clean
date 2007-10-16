;nyquist plug-in
;version 1
;type process
;name "Tremolo..."
;action "Applying Tremolo..."
;info "Released under terms of the GNU General Public License Version 2\n'Wetness level' controls the proportion of the original audio fed back\ninto the result.\n'Frequency' controls the rapidity of the oscillation, higher frequencies\nbeing more rapid."

;control amount "Wetness level [%]" int "" 40 0 100
;control lfo "Frequency [Hz]" real "" 4.0 0.1 10.0
(mult (sum (const (- 1.0 (/ amount 200.0))) (scale (/ amount 200.0) (osc (hz-to-step lfo)))) s)

; arch-tag: 0ee3925a-8016-44db-91e8-8c4b7a9f3992

