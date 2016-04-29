import scala.util.{Success, Failure}
import scala.concurrent.{ Future, Promise }
import scala.concurrent.ExecutionContext.Implicits.global
object HelloWorld {

  def futureadder(finput: Int) : Future[Either[String, Int]] = {
    val s = "ERROR"
    val p = Promise[Either[String, Int]]()
    val f: Future[Either[String, Int]] = Future {

      //p.success(adder(finput))
      //p.success(s)i
      Right(adder(finput))

    }

    f.onComplete {
      case Success(s) => p success Right(finput)
      case Failure(s) => p success Left("ERROR")
    }

    p.future
  }

  def anotheradder(anotherinput: Int) = {
    anotherinput + 1

  }
  def adder(input: Int): Int = {
    return anotheradder(input) + 1
  }
  def main(args: Array[String]): Unit = {
    println("Hello, world!")
    println(adder(2))
    println(futureadder(adder(2)))
  }
}
