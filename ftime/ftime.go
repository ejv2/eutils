// ftime - print formatted time to the console
package main

import (
	"fmt"
	"time"
	"flag"
)

var (
	// Format is the time.Format used for the output.
	Format = time.UnixDate
	// StrFormat is the input format from the user.
	StrFormat = flag.String("format", "", "Name of the format to use, as used in time.Format")

	Layout = flag.Bool("layout", false, "")
	ANSIC = flag.Bool("ansic", false, "")
	UnixDate = flag.Bool("unix", false, "")
	RubyDate = flag.Bool("ruby", false, "")
	RFC822 = flag.Bool("rfc822", false, "")
	RFC850 = flag.Bool("rfc850", false, "")
	RFC1123 = flag.Bool("rfc1123", false, "")
	RFC1123Z = flag.Bool("rfc1123z", false, "")
	RFC3339 = flag.Bool("rfc3339", false, "")
	RFC3339Nano = flag.Bool("rfc3339nano", false, "")
	Kitchen = flag.Bool("kitchen", false, "")
	StampMilli = flag.Bool("stampmilli", false, "")
	StampMicro = flag.Bool("stampmicro", false, "")
	StampNano = flag.Bool("stampnano", false, "")
	DateTime = flag.Bool("datetime", false, "")
	DateOnly = flag.Bool("date", false, "")
	TimeOnly = flag.Bool("time", false, "")
)

func parseFormat() {
	switch *StrFormat {
	case "Layout":
		Format = time.Layout
	case "ANSIC":
		Format = time.ANSIC
	case "UnixDate":
		Format = time.UnixDate
	case "RubyDate":
		Format = time.RubyDate
	case "RFC822":
		Format = time.RFC822
	case "RFC850":
		Format = time.RFC850
	case "RFC1123":
		Format = time.RFC1123
	case "RFC1123Z":
		Format = time.RFC1123Z
	case "RFC3339":
		Format = time.RFC3339
	case "RFC3339Nano":
		Format = time.RFC3339Nano
	case "Kitchen":
		Format = time.Kitchen
	case "StampMilli":
		Format = time.StampMilli
	case "StampMicro":
		Format = time.StampMicro
	case "StampNano":
		Format = time.StampNano
	case "DateTime":
		Format = time.DateTime
	case "DateOnly":
		Format = time.DateOnly
	case "TimeOnly":
		Format = time.TimeOnly
	}
}

func parseFlags() {
	switch {
	case *Layout:
		Format = time.Layout
	case *ANSIC:
		Format = time.ANSIC
	case *UnixDate:
		Format = time.UnixDate
	case *RubyDate:
		Format = time.RubyDate
	case *RFC822:
		Format = time.RFC822
	case *RFC850:
		Format = time.RFC850
	case *RFC1123:
		Format = time.RFC1123
	case *RFC1123Z:
		Format = time.RFC1123Z
	case *RFC3339:
		Format = time.RFC3339
	case *RFC3339Nano:
		Format = time.RFC3339Nano
	case *Kitchen:
		Format = time.Kitchen
	case *StampMilli:
		Format = time.StampMilli
	case *StampMicro:
		Format = time.StampMicro
	case *StampNano:
		Format = time.StampNano
	case *DateTime:
		Format = time.DateTime
	case *DateOnly:
		Format = time.DateOnly
	case *TimeOnly:
		Format = time.TimeOnly
	}
}

func main() {
	flag.Parse()
	parseFormat()
	parseFlags()

	fmt.Println(time.Now().Format(Format))
}
