// Copyright (C) 2022 - Ethan Marshall

// ttr is a time tracker to track long-running tasks or projects in a sort of
// timeline. It automatically formats a text file, which is designed to be both
// human readable and machine-parseable.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"os"
	"runtime"
	"sort"
	"strings"
	"time"
)

const (
	TrackerFilepath = "ttime"
)

var (
	trackerFile = flag.String("file", "$XDG_DATA_HOME/"+TrackerFilepath, "The location of the time tracker file")
	newFlag     = flag.String("now", "", "You are now doing this. Create a new entry with this title")
	doneFlag    = flag.String("done", "", "You have now done this. Mark the this entry as done")
	clearFlag   = flag.Bool("clear", false, "Clear all previous time trackers")
)

func init() {
	aliases := map[string]string{
		"now":  "start",
		"done": "stop",
	}

	for from, to := range aliases {
		flagSet := flag.Lookup(from)
		flag.Var(flagSet.Value, to, fmt.Sprintf("Alias to %s", flagSet.Name))
	}
}

func getDataHome() (string, error) {
	bs, err := os.UserHomeDir()
	if err != nil {
		return "", fmt.Errorf("open tracker file: %w", err)
	}

	if runtime.GOOS == "windows" {
		return bs, nil
	}

	xdg := os.Getenv("XDG_DATA_HOME")
	if xdg == "" {
		xdg = bs + string(os.PathSeparator) + ".local/share"
	}
	return xdg, nil
}

// A Tracker is a single instance of a time tracker. It is essentially one row
// in the time tracker file.
type Tracker struct {
	// The title of this time tracker
	Title      string
	Ongoing    bool
	Start, End time.Time
	Lasted     time.Duration
}

func (t Tracker) String() string {
	end := t.End.Format(time.ANSIC)
	if t.Ongoing {
		end = "..."
	}
	base := fmt.Sprint(t.Title, " @ ", t.Start.Format(time.ANSIC), " --> ", end)
	if !t.Ongoing {
		base += fmt.Sprintf(" (%v)", t.Lasted)
	}
	return base
}

func (t Tracker) Less(o Tracker) bool {
	return t.Title < o.Title
}

// A TrackerFile contains alphabetically sorted entries of time tracker
// instances, formatted according to Tracker.String.
type TrackerFile struct {
	Path     string
	Trackers []Tracker
}

func NewTrackerFile(path string) (TrackerFile, error) {
	f, err := os.Open(path)
	if err != nil {
		if os.IsNotExist(err) {
			f, err = os.Create(path)
			if err != nil {
				return TrackerFile{}, err
			}
		}
		return TrackerFile{}, err
	}
	defer f.Close()

	track := TrackerFile{
		Path: path,
	}
	scan := bufio.NewScanner(f)
	for scan.Scan() {
		line := scan.Text()
		if line == "" {
			continue
		}

		halves := strings.Split(line, " --> ")
		if len(halves) != 2 {
			return track, fmt.Errorf("syntax error: expected '--->' separator")
		}

		var err error
		tracker := Tracker{}

		// Parse first half
		fquarter := strings.Split(halves[0], " @ ")
		if len(fquarter) != 2 {
			return track, fmt.Errorf("syntax error: expected '@' sub-separator")
		}
		tracker.Title = fquarter[0]
		tracker.Start, err = time.Parse(time.ANSIC, fquarter[1])
		if err != nil {
			return track, fmt.Errorf("syntax error: invalid start date format: %w", err)
		}

		if halves[1] == "..." {
			tracker.Ongoing = true
		} else {
			strip := strings.IndexRune(halves[1], '(')
			if strip <= 0 {
				strip = len(halves[1])
			}
			work := halves[1][:strip-1]

			tracker.End, err = time.Parse(time.ANSIC, work)
			if err != nil {
				return track, fmt.Errorf("syntax error: invalid end date format: %w", err)
			}
			tracker.Lasted = tracker.End.Sub(tracker.Start)
		}

		track.Trackers = append(track.Trackers, tracker)
	}

	sort.Sort(&track)

	return track, nil
}

func (t *TrackerFile) Insert(title string) {
	ins := Tracker{
		Title:   title,
		Ongoing: true,
		Start:   time.Now(),
	}

	if tr := t.Find(title); tr != nil {
		*tr = ins
		return
	}

	t.Trackers = append(t.Trackers, ins)
}

func (t *TrackerFile) Find(title string) *Tracker {
	if len(t.Trackers) == 0 {
		return nil
	}

	match := func(i int) bool { return t.Trackers[i].Title >= title }
	found := sort.Search(len(t.Trackers), match)
	if found == -1 {
		return nil
	}
	if t.Trackers[found].Title != title {
		return nil
	}

	return &t.Trackers[found]
}

func (t *TrackerFile) Len() int {
	return len(t.Trackers)
}

func (t *TrackerFile) Less(i, j int) bool {
	return t.Trackers[i].Less(t.Trackers[j])
}

func (t *TrackerFile) Swap(i, j int) {
	t.Trackers[i], t.Trackers[j] = t.Trackers[j], t.Trackers[i]
}

func (t *TrackerFile) Marshal(w io.Writer) error {
	sort.Sort(t)

	for _, tr := range t.Trackers {
		if _, err := fmt.Fprintln(w, tr); err != nil {
			return err
		}
	}

	return nil
}

func insertNew(t *TrackerFile) {
	t.Insert(*newFlag)
}

func markDone(t *TrackerFile) error {
	ent := t.Find(*doneFlag)
	if ent == nil {
		return fmt.Errorf("%s: entry not found", *doneFlag)
	}

	ent.Ongoing = false
	ent.End = time.Now()
	ent.Lasted = ent.End.Sub(ent.Start)

	return nil
}

func reserialize(tr *TrackerFile) error {
	f, err := os.Create(*trackerFile)
	if err != nil {
		return err
	}
	defer f.Close()

	if *clearFlag {
		return nil
	}

	if err := tr.Marshal(f); err != nil {
		return err
	}

	return nil
}

func main() {
	flag.Parse()

	// Preprocess file to replace references to data home
	h, err := getDataHome()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	*trackerFile = strings.Replace(*trackerFile, "$XDG_DATA_HOME", h, 1)
	tf, err := NewTrackerFile(*trackerFile)
	if err != nil {
		fmt.Println("error parsing tracker file:", err)
		os.Exit(1)
	}
	defer func() {
		if err := reserialize(&tf); err != nil {
			fmt.Println("failed to write tracker file:", err)
			os.Exit(1)
		}
	}()

	if *newFlag != "" {
		insertNew(&tf)
	} else if *doneFlag != "" {
		if err := markDone(&tf); err != nil {
			fmt.Println(err)
			os.Exit(1)
		}
	} else if *clearFlag {
		return
	} else {
		for _, e := range tf.Trackers {
			fmt.Println(e)
		}
	}
}
