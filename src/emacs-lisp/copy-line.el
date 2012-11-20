;;; copy-line.el --- 
;; 
;; Filename: copy-line.el
;; Description: 
;; Author: the machine of awareness
;; Maintainer: 
;; Created: 二 11月 13 09:26:14 2012 (+0800)
;; Version: 
;; Last-Updated: 
;;           By: 
;;     Update #: 0
;; URL: 
;; Doc URL: 
;; Keywords: 
;; Compatibility: 
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;;; Commentary: 
;; 
;; 
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;;; Change Log:
;; 
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License as
;; published by the Free Software Foundation; either version 3, or
;; (at your option) any later version.
;; 
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.
;; 
;; You should have received a copy of the GNU General Public License
;; along with this program; see the file COPYING.  If not, write to
;; the Free Software Foundation, Inc., 51 Franklin Street, Fifth
;; Floor, Boston, MA 02110-1301, USA.
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 
;;; Code:


(defun copy-line1 (arg)
  "Use kill-ring-save."
  (interactive "p")
  (kill-ring-save (line-beginning-position 1)
                  (line-beginning-position (+ 1 arg)))
  (message "%d line%s copied." arg (if (= 1 arg) "" "s")))

(defun copy-line2 (&optional arg)
  "Use kill-line, set buffer-read-only t, and kill-read-only-ok t."
  (interactive "P")
  (let ((buffer-read-only t)
        (kill-read-only-ok t))
    (kill-line arg)))
               
(defun copy-line3 ()
  "Use kill-append, kill-new."
  (interactive)
  (let ((beg (line-beginning-position 1))
        (end (line-beginning-position 2)))
    (if (eq last-command 'copy-line3)
        (kill-append (buffer-substring beg end) (< end beg))
      (kill-new (buffer-substring beg end))))
  (beginning-of-line 2))

(global-set-key (kbd "s-c") 'copy-line3)

(defun kill-line1 ()
  "Use kill-append, kill-new."
  (interactive)
  (let ((beg (line-beginning-position 1))
        (end (line-beginning-position 2)))
    (if (eq last-command 'kill-line1)
        (kill-append (buffer-substring beg end) (< end beg))
      (kill-new (buffer-substring beg end)))
    (delete-region beg end))
  (beginning-of-line 1))

(global-set-key (kbd "s-x") 'kill-line1)

(defun get-point (symbol &optional arg)
  "thisandthat."
  (funcall symbol (if (listp arg) (car arg) arg))
  (point))

(defun copy-thing (begin-of-thing en-of-thing &optional arg)
  "thisandthat."
  (let ((beg (get-point begin-of-thing 1))
        (end (get-point en-of-thing arg)))
    (kill-ring-save beg end)))

(defun paste-to-mark ()
  "thisandthat."
  (goto-char (mark))
  (yank))

(defun copy-line4 (&optional arg)
  "Use copy-thing."
  (interactive "P")
  (copy-thing 'beginning-of-line 'end-of-line arg)
  (paste-to-mark))

(defun copy-word (&optional arg)
  "thisandthat."
  (interactive "P")
  (copy-thing 'backward-word 'forward-word arg)
  (paste-to-mark))

(defun beginning-of-parenthese (&optional arg)
  "thisandthat."
  (re-search-backward "[\[({]" (line-beginning-position) 3 1)
  (if (looking-at "[\[({]") (goto-char (+ (point) 1))))

(defun end-of-parenthese (&optional arg)
  "thisandthat."
  (re-search-forward "[\])}]" (line-end-position) 3 arg)
  (if (looking-back "[\])}]") (goto-char (- (point) 1))))

(defun copy-parenthese (&optional arg)
  "thisandthat."
  (interactive "P")
  (copy-thing 'beginning-of-parenthese 'end-of-parenthese arg)
  (paste-to-mark))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; copy-line.el ends here
